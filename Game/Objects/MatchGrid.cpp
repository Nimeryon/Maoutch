#include <iostream>
#include <fstream>

#include "MatchGrid.h"
#include "MatchGridBackGround.h"
#include "../Effects/ObjectShaker.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Assets.h"
#include "../../Engine/Graphics/ParticleEmitter.h"
#include "../../Types/Direction.h"
#include "MatchHint.h"

namespace maoutch
{
	MatchGrid::MatchGrid(const std::string& fileName, const Vector2& position, const Element& element) :
		GameObject("Match Grid", 0),
		_matchGridBackGround(new MatchGridBackGround(element)),
		_emptyPositions({}),
		_state(GridState::Starting),
		_matchFinder(&_grid),
		_moveChecked(false),
		_lastSwapDir(Direction::DirectionValue::None),
		_startTimer(Assets::Config<float>("Element", "MoveTime"), &MatchGrid::_Start, this),
		_destroyTimer(Assets::Config<float>("Grid", "DestroyTime"), &MatchGrid::_DestroyMatched, this),
		_collapseTimer(Assets::Config<float>("Element", "MoveTime") + Assets::Config<float>("Grid", "CollapseTime"), &MatchGrid::_CollapseColumns, this),
		_refillTimer(Assets::Config<float>("Element", "MoveTime") + Assets::Config<float>("Grid", "RefillTime"), &MatchGrid::_RefillBoard, this),
		_afterRefillTimer(0, &MatchGrid::_AfterRefill, this),
		_startResetTimer(Assets::Config<float>("Element", "MoveTime"), &MatchGrid::StartReset, this),
		_endResetTimer(Assets::Config<float>("Element", "MoveTime") + Assets::Config<float>("Grid", "ResetMaxMoveTime"), &MatchGrid::_Reset, this),
		_swapBackTimer(Assets::Config<float>("Element", "MoveTime") + Assets::Config<float>("Grid", "SwapBackTime"), &MatchGrid::_SwapBack, this),
		_processMatchTimer(Assets::Config<float>("Element", "MoveTime") + Assets::Config<float>("Grid", "ProcessMatchTime"), &MatchGrid::_ProcessMatches, this),
		_showHintTimer(Assets::Config<float>("Grid", "ShowHintTime"), &MatchGrid::_SetPossibleMatch, this)
	{
		SetScale(Assets::Config<float>("Grid", "Scale"));
		SetPosition(position);
		Setup(fileName);

		_matchGridBackGround->Setup(_grid);
		AddChildren(_matchGridBackGround);

		AddChildren(new MatchHint());
	}

	void MatchGrid::Setup(const std::string& fileName)
	{
		std::ifstream file(Assets::Config<std::string>("Grid", "Path") + fileName);
		std::string line;

		getline(file, line);
		const Vector2i gridSize = Vector2i::FromString(line);

		// Empty Pos
		for (int y = 0; y < gridSize.y; ++y)
		{
			getline(file, line);
			std::vector<std::string> values = string::Split(line, ',');
			for (int x = 0; x < gridSize.x; ++x)
				if (values[x] == "0") _emptyPositions.emplace_back(Vector2i(x, y));
		}

		// Respawn Points
		getline(file, line);
		const int respawnPoints = std::atoi(&line[0]);
		for (int i = 0; i < respawnPoints; ++i)
		{
			getline(file, line);
			_respawnPoints.emplace_back(Vector2::FromString(line));
		}

		// Shake Parameters
		getline(file, line);
		std::vector<std::string> values = string::Split(line, ',');
		_shakeMinTime = std::atoi(&values[0][0]);
		_shakeMaxTime = std::atoi(&values[1][0]);
		_shakeMaxRadius = std::atoi(&values[2][0]);
		_shakeMagnitude = std::atoi(&values[3][0]);
		_shakeDecreaseFactor = std::atoi(&values[4][0]);

		SetupGrid(gridSize);
	}
	void MatchGrid::SetupGrid(const Vector2i& gridSize)
	{
		_grid.Resize(gridSize.x, gridSize.y);
		for (int i = 0;i < gridSize.x * gridSize.y; ++i) 
			_grid.EmplaceBack(nullptr);

		_FillGrid();
		UpdateElementsPosition(
			Assets::Config<float>("Grid", "SetupMinFallTime"),
			Assets::Config<float>("Grid", "SetupMaxFallTime"),
			easing::EaseType::EaseOutCubic
		);

		_showHintTimer.Restart();
		_startTimer.Start();
	}
	void MatchGrid::SetupGridPos(const Vector2i& gridPos)
	{
		if (!_grid.GetGridElement(gridPos)) return;

		Element element = _grid.GetGridElement(gridPos)->GetElement();

		// Change element until no match found with limit of 100 pass
		int count = 0;
		while (_matchFinder.MatchAt(gridPos, element) && count < 100)
		{
			element = Element::Random();
			count++;
		}

		_grid.GetGridElement(gridPos)->SetElement(element);
	}

	bool MatchGrid::IsMoving() const
	{
		return std::any_of(
			_grid.array.begin(),
			_grid.array.end(),
			[](const MatchElement* element) { return element && element->IsMoving(); });
	}
	bool MatchGrid::IsValidGridPosition(const Vector2i& gridPos) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid.GetWidth() || gridPos.y >= _grid.GetHeight()) return false;
		return std::find(_emptyPositions.begin(), _emptyPositions.end(), gridPos) == _emptyPositions.end();
	}

	void MatchGrid::StartReset()
	{
		SetState(GridState::Moving);

		_DisableMatchHint();

		for (Vector2 respawnPoint : _respawnPoints)
			_SpawnParticle(Assets::Config<std::string>("Particle", "Path") + "Elements_particle", GetCenterGridPosition(respawnPoint));

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				Vector2 randomRespawnPoint = _respawnPoints[random::Int(0, _respawnPoints.size())];
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos))
					_grid.GetGridElement(gridPos)->MoveToPos(
						GetCenterGridPosition(randomRespawnPoint),
						Assets::Config<float>("Grid", "ResetMinMoveTime"),
						Assets::Config<float>("Grid", "ResetMaxMoveTime"),
						easing::EaseType::EaseInBack,
						true
					);
			}

		ObjectShaker::Shake(
			this,
			random::Float(
				_shakeMinTime,
				_shakeMaxTime
			),
			_shakeMaxRadius,
			_shakeMagnitude,
			_shakeDecreaseFactor
		);

		_endResetTimer.Start();
	}

	void MatchGrid::Swap(const Vector2i gridPos, const Direction dir)
	{
		SetState(GridState::Moving);

		Vector2i goalPos = gridPos + dir;
		if (!IsValidGridPosition(goalPos))
		{
			SetState(GridState::Inputs);
			return;
		}
		if (!_grid.GetGridElement(gridPos) || !_grid.GetGridElement(goalPos)) return;

		_DisableMatchHint();

		_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(goalPos, 0, easing::EaseType::EaseInOutQuart);
		_grid.GetGridElement(goalPos)->SetAndMoveToGridPos(gridPos, 0, easing::EaseType::EaseInOutQuart);

		// Swap elements memory
		std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(goalPos));

		_lastSwapGoalPos = goalPos;
		_lastSwapDir = dir;

		if (!_moveChecked) _processMatchTimer.Start();
	}
	void MatchGrid::DestroyGridPos(const Vector2i& gridPos)
	{
		if (!IsValidGridPosition(gridPos) || !_grid.GetGridElement(gridPos) ) return;

		GameObjectHandler::instance->Destroy(_grid.GetGridElement(gridPos));
		_grid.GetGridElement(gridPos) = nullptr;
	}

	void MatchGrid::UpdateElementsPosition(const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType)
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos))
					_grid.GetGridElement(gridPos)->MoveToGridPos(minStartMoveTime, maxStartMoveTime, easeType);
			}
		}
	}
	void MatchGrid::UpdateElementsPosition(const float& moveTime, const easing::EaseType& easeType)
	{
		UpdateElementsPosition(moveTime, moveTime, easeType);
	}

	Vector2 MatchGrid::GetCenterGridPosition(const Vector2& gridPos) const
	{
		const float halfWidth = (_grid.GetWidth() / 2.f) - .5f;
		const float halfHeight = (_grid.GetHeight() / 2.f) - .5f;

		Vector2 offsetPos = gridPos - Vector2(halfWidth, halfHeight);
		const float elementSize = Assets::Config<float>("Element", "Size");
		return offsetPos * elementSize;
	}
	GridState MatchGrid::GetState() const { return _state; }
	void MatchGrid::SetState(const GridState& state) { _state = state; }

	void MatchGrid::PrintGrid()
	{
		std::string value = "";
		for (int y = 0; y < _grid.GetHeight(); ++y)
		{
			for (int x = 0; x < _grid.GetWidth(); ++x)
			{
				if (x != 0) value += " , ";

				if (!_grid.GetGridElement(Vector2i(x, y))) value += "null";
				else value += std::to_string((int)_grid.GetGridElement(Vector2i(x, y))->GetElement().Value());
			}
			value += "\n";
		}
		std::cout << value << std::endl;
	}

	void MatchGrid::_DestroyMatched()
	{
		bool matched = false;
		float matchedCount = 0;

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos))
					if (_grid.GetGridElement(gridPos) && _grid.GetGridElement(gridPos)->IsMatched())
					{
						matched = true;
						matchedCount++;
						_SpawnParticle(_grid.GetGridElement(gridPos)->GetElement(), gridPos);
						DestroyGridPos(gridPos);
					}
			}
		}
		_moveChecked = true;

		if (matched)
		{
			ObjectShaker::Shake(this,
				Assets::Config<float>("Grid", "ShakeTime") * matchedCount,
				Assets::Config<float>("Grid", "ShakeMaxRadius") * matchedCount,
				Assets::Config<float>("Grid", "ShakeMagnitude") * matchedCount,
				Assets::Config<float>("Grid", "ShakeDecreaseFactor")
			);
			_collapseTimer.Start();
		}
		else _swapBackTimer.Start();
	}
	void MatchGrid::_CollapseColumns()
	{
		int nullCount = 0;
		Vector2i gridPos;
		for (gridPos.x = _grid.GetWidth() - 1; gridPos.x >= 0; --gridPos.x)
		{
			for (gridPos.y = _grid.GetHeight() - 1; gridPos.y >= 0; --gridPos.y)
			{
				const bool validPos = IsValidGridPosition(gridPos);
				if (validPos && !_grid.GetGridElement(gridPos))
					nullCount++;
				else if (nullCount > 0)
				{
					if (!validPos) break;

					Vector2i pos = { gridPos.x, gridPos.y + nullCount };
					_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(pos, 0, easing::EaseType::EaseInOutQuart);

					// Swap elements memory
					std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(pos));
				}
			}
			nullCount = 0;
		}

		_refillTimer.Start();
	}
	void MatchGrid::_RefillBoard()
	{
		float moveTimeDelay = 0;

		Vector2i gridPos;
		for (gridPos.y = _grid.GetHeight() - 1; gridPos.y >= 0; --gridPos.y)
		{
			bool lineRefilled = false;
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos) && !_grid.GetGridElement(gridPos))
				{
					Vector2 position = GetCenterGridPosition(gridPos);
					_grid.GetGridElement(gridPos) = new MatchElement(*this, Vector2(position.x, -Assets::Config<float>("Window", "Height") - position.y), gridPos, Element::Random());
					_grid.GetGridElement(gridPos)->SetName((std::string)gridPos + " Element");
					AddChildren(_grid.GetGridElement(gridPos));

					_grid.GetGridElement(gridPos)->MoveToGridPos(moveTimeDelay, 0, easing::EaseType::EaseInOutQuart);

					lineRefilled = true;
				}
			}
			if (lineRefilled) moveTimeDelay += Assets::Config<float>("Grid", "RefillDelayTime");
		}

		_afterRefillTimer.SetTime(moveTimeDelay);
		_afterRefillTimer.Start();
	}
	void MatchGrid::_AfterRefill()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos))
					if (_matchFinder.MatchAt(gridPos, _grid.GetGridElement(gridPos)->GetElement()))
					{
						_processMatchTimer.Start();
						return;
					}
		_ProcessPossibleMatches();
	}
	void MatchGrid::_Reset()
	{
		_moveChecked = false;
		SetState(GridState::Starting);

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				DestroyGridPos(gridPos);

		_FillGrid(true);
		UpdateElementsPosition(
			Assets::Config<float>("Grid", "ResetMinMoveTime"),
			Assets::Config<float>("Grid", "ResetMaxMoveTime"),
			easing::EaseType::EaseOutBack
		);

		_showHintTimer.Restart();
		_startTimer.Start();
	}

	void MatchGrid::_ProcessMatches()
	{
		_matchFinder.FindMatches();
		for (Match& match : _matchFinder.matches)
			for (Vector2i gridPos : match.positions)
				if (_grid.GetGridElement(gridPos))
					_grid.GetGridElement(gridPos)->SetIsMatched();

		_destroyTimer.Start();
	}
	void MatchGrid::_ProcessPossibleMatches()
	{
		_matchFinder.FindPossibleMatches();
		if (_matchFinder.possibleMatches.empty()) {
			Vector2i gridPos;
			for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
				for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
					if (_grid.GetGridElement(gridPos))
						_grid.GetGridElement(gridPos)->SetIsMatched();

			_startResetTimer.Start();
		}
		else
		{
			_showHintTimer.Restart();
			_moveChecked = false;
			SetState(GridState::Inputs);
		}
	}

	void MatchGrid::_Start()
	{
		SetState(GridState::Inputs);
	}
	void MatchGrid::_DisableMatchHint()
	{
		_showHintTimer.Reset();
		GetChildren("MatchHint")->SetActive(false);
	}
	void MatchGrid::_SetPossibleMatch()
	{
		// Set Possible Match Hint
		auto matchHint = (MatchHint*)GetChildren("MatchHint");
		matchHint->SetActive(true);
		matchHint->SetPossibleMatch(_matchFinder.possibleMatches[random::Int(0, _matchFinder.possibleMatches.size())]);

	}
	void MatchGrid::_SwapBack()
	{
		if (_grid.GetGridElement(_lastSwapGoalPos) && _grid.GetGridElement(_lastSwapGoalPos + !_lastSwapDir))
			Swap(_lastSwapGoalPos, !_lastSwapDir);

		_moveChecked = false;
		_startTimer.Start();
	}
	void MatchGrid::_FillGrid(bool createAtCenter)
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				if (IsValidGridPosition(gridPos))
				{
					Vector2 position = GetCenterGridPosition(gridPos);
					Vector2 randomRespawnPoint = _respawnPoints[random::Int(0, _respawnPoints.size())];
					_grid.GetGridElement(gridPos) = new MatchElement(
						*this,
						createAtCenter ? GetCenterGridPosition(randomRespawnPoint) : Vector2(position.x, -Assets::Config<float>("Window", "Height") - position.y),
						gridPos,
						Element::Random()
					);

					_grid.GetGridElement(gridPos)->SetName((std::string)gridPos + " Element");
					AddChildren(_grid.GetGridElement(gridPos));

					SetupGridPos(gridPos);
				}

		while (!_matchFinder.FindPossibleMatches())
			for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
				for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
					if (IsValidGridPosition(gridPos))
					{
						_grid.GetGridElement(gridPos)->SetElement(Element::Random());
						SetupGridPos(gridPos);
					}
	}
	void MatchGrid::_SpawnParticle(const Element& element, const Vector2i& gridPos)
	{
		const std::string particleFileName = Assets::Config<std::string>("Particle", "Path") + "Elements\\" + element.ToString() + "_particle";
		
		ParticleEmitter* emitter = new ParticleEmitter();
		emitter->SetZIndex(3);
		emitter->SetPosition(GetCenterGridPosition(gridPos));
		emitter->SetupFromFile(particleFileName);

		AddChildren(emitter);
	}
	void MatchGrid::_SpawnParticle(const std::string& fileName, const Vector2& position)
	{
		ParticleEmitter* emitter = new ParticleEmitter();
		emitter->SetZIndex(3);
		emitter->SetPosition(position);
		emitter->SetupFromFile(fileName);

		AddChildren(emitter);
	}
}
