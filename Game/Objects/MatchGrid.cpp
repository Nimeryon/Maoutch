#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "MatchGrid.h"
#include "MatchGridBackGround.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Graphics/ParticleEmitter.h"
#include "../../Types/Direction.h"
#include "MatchHint.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern maoutch::Vector2 ELEMENT_SIZE;
extern maoutch::Vector2 ELEMENT_SCALE;

namespace maoutch
{
	MatchGrid::MatchGrid(const std::string& fileName, const Vector2& position) :
		GameObject("Match Grid", 0),
		_matchGridBackGround(new MatchGridBackGround()),
		_emptyPositions({}),
		_state(GridState::STARTING),
		_matchFinder(&_grid),
		_moveChecked(false),
		_lastSwapDir(Direction::DirectionValue::NONE),
		_destroyTimer(destroyTime, &MatchGrid::_DestroyMatched, this),
		_collapseTimer(collapseTime, &MatchGrid::_CollapseColumns, this),
		_refillTimer(refillTime, &MatchGrid::_RefillBoard, this),
		_afterRefillTimer(afterRefillTime, &MatchGrid::_AfterRefill, this),
		_startResetTimer(startResetTime, &MatchGrid::StartReset, this),
		_endResetTimer(endResetTime, &MatchGrid::_Reset, this),
		_swapBackTimer(swapBackTime, &MatchGrid::_SwapBack, this),
		_processMatchTimer(processMatchTime, &MatchGrid::_ProcessMatches, this),
		_processPossibleMatchTimer(processPossibleMatchTime, &MatchGrid::_ProcessPossibleMatches, this),
		_showHintTimer(showHintTime, &MatchGrid::_SetPossibleMatch, this)
	{
		SetScale(ELEMENT_SCALE);
		SetPosition(position);
		Setup(fileName);

		_matchGridBackGround->Setup(_grid);
		AddChildren(_matchGridBackGround);

		AddChildren(new MatchHint());
	}

	void MatchGrid::FixedUpdate(float dt)
	{
		switch (_state)
		{
			case GridState::STARTING:
				if (!IsMoving())
					SetState(GridState::INPUTS);
				break;

			default: break;
		}	
	}

	void MatchGrid::Setup(const std::string& fileName)
	{
		std::ifstream file(gridPath + fileName);
		std::string line;

		getline(file, line);
		const Vector2i gridSize = Vector2i::FromString(line);
		
		for (int y = 0; y < gridSize.y; ++y)
		{
			getline(file, line);
			std::vector<std::string> values = string::Split(line, ',');
			for (int x = 0; x < gridSize.x; ++x)
				if (values[x] == "0") _emptyPositions.emplace_back(Vector2i(x, y));
		}

		SetupGrid(gridSize);
	}
	void MatchGrid::SetupGrid(const Vector2i& gridSize)
	{
		_grid.Resize(gridSize.x, gridSize.y);
		for (int i = 0;i < gridSize.x * gridSize.y; ++i) 
			_grid.EmplaceBack(nullptr);

		_FillGrid();
		UpdateElementsPosition(setupMinFallTime, setupMaxFallTime);

		_showHintTimer.Restart();
	}
	void MatchGrid::SetupGridPos(const Vector2i& gridPos)
	{
		if (_grid.GetGridElement(gridPos) == nullptr) return;

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
			[](const MatchElement* element) { return element != nullptr && element->IsMoving(); });
	}
	bool MatchGrid::IsValidGridPosition(const Vector2i& gridPos) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid.GetWidth() || gridPos.y >= _grid.GetHeight()) return false;
		return std::find(_emptyPositions.begin(), _emptyPositions.end(), gridPos) == _emptyPositions.end();
	}

	void MatchGrid::StartReset()
	{
		SetState(GridState::MOVING);

		_DisableMatchHint();
		_SpawnParticle(particlesPath + "Elements_particle", Vector2::Zero());

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos) != nullptr)
					_grid.GetGridElement(gridPos)->MoveToPos(Vector2(0, 0), 0, .5f);

		_endResetTimer.Start();
	}

	void MatchGrid::Swap(const Vector2i gridPos, const Direction dir)
	{
		SetState(GridState::MOVING);

		Vector2i goalPos = gridPos + dir;
		if (!IsValidGridPosition(goalPos))
		{
			SetState(GridState::INPUTS);
			return;
		}
		if (_grid.GetGridElement(gridPos) == nullptr || _grid.GetGridElement(goalPos) == nullptr) return;

		_DisableMatchHint();

		_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(goalPos);
		_grid.GetGridElement(goalPos)->SetAndMoveToGridPos(gridPos);

		// Swap elements memory
		std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(goalPos));

		_lastSwapGoalPos = goalPos;
		_lastSwapDir = dir;

		if (!_moveChecked) _ProcessMatches();
	}
	void MatchGrid::DestroyGridPos(const Vector2i& gridPos)
	{
		if (!IsValidGridPosition(gridPos) || _grid.GetGridElement(gridPos) == nullptr) return;

		GameObjectHandler::instance->Destroy(_grid.GetGridElement(gridPos));
		_grid.GetGridElement(gridPos) = nullptr;
	}

	void MatchGrid::UpdateElementsPosition(const float& minStartMoveTime, const float& maxStartMoveTime)
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) != nullptr)
					_grid.GetGridElement(gridPos)->MoveToGridPos(minStartMoveTime, maxStartMoveTime);
			}
		}
	}
	void MatchGrid::UpdateElementsPosition(const float& moveTime)
	{
		UpdateElementsPosition(moveTime, moveTime);
	}

	Vector2 MatchGrid::GetCenterGridPosition(const Vector2i& gridPos) const
	{
		const float halfWidth = (_grid.GetWidth() / 2.f) - .5f;
		const float halfHeight = (_grid.GetHeight() / 2.f) - .5f;

		Vector2 offsetPos = gridPos - Vector2(halfWidth, halfHeight);
		return offsetPos * ELEMENT_SIZE;
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

				if (_grid.GetGridElement(Vector2i(x, y)) == nullptr) value += "null";
				else value += std::to_string((int)_grid.GetGridElement(Vector2i(x, y))->GetElement().Value());
			}
			value += "\n";
		}
		std::cout << value << std::endl;
	}

	void MatchGrid::_DestroyMatched()
	{
		bool matched = false;

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos))
					if (_grid.GetGridElement(gridPos) != nullptr && _grid.GetGridElement(gridPos)->IsMatched())
					{
						matched = true;
						_SpawnParticle(_grid.GetGridElement(gridPos)->GetElement(), gridPos);
						DestroyGridPos(gridPos);
					}
			}
		}

		_moveChecked = true;

		if (matched) _collapseTimer.Start();
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
				if (validPos && _grid.GetGridElement(gridPos) == nullptr)
					nullCount++;
				else if (nullCount > 0)
				{
					if (!validPos) break;

					Vector2i pos = { gridPos.x, gridPos.y + nullCount };
					_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(pos);

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
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos) == nullptr)
				{
					Vector2 position = GetCenterGridPosition(gridPos);
					_grid.GetGridElement(gridPos) = new MatchElement(*this, Vector2(position.x, -WINDOW_HEIGHT - position.y), gridPos, Element::Random());
					_grid.GetGridElement(gridPos)->SetName((std::string)gridPos + " Element");
					AddChildren(_grid.GetGridElement(gridPos));

					_grid.GetGridElement(gridPos)->MoveToGridPos(moveTimeDelay);

					lineRefilled = true;
				}
			}
			if (lineRefilled) moveTimeDelay += refillDelayTime;
		}

		_afterRefillTimer.SetTime(afterRefillTime + moveTimeDelay);
		_afterRefillTimer.Start();
	}
	void MatchGrid::_AfterRefill()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos) != nullptr)
					if (_matchFinder.MatchAt(gridPos, _grid.GetGridElement(gridPos)->GetElement()))
					{
						_processMatchTimer.Start();
						return;
					}

		_processPossibleMatchTimer.Start();
	}
	void MatchGrid::_Reset()
	{
		_moveChecked = false;
		SetState(GridState::STARTING);

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				DestroyGridPos(gridPos);

		_FillGrid(true);
		UpdateElementsPosition(resetMinMoveTime, resetMaxMoveTime);

		_showHintTimer.Restart();
	}

	void MatchGrid::_ProcessMatches()
	{
		_matchFinder.FindMatches();
		for (Match& match : _matchFinder.matches)
			for (Vector2i gridPos : match.positions)
				if (_grid.GetGridElement(gridPos) != nullptr)
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
					if (_grid.GetGridElement(gridPos) != nullptr)
						_grid.GetGridElement(gridPos)->SetIsMatched();

			_startResetTimer.Start();
		}
		else
		{
			_showHintTimer.Restart();
			_moveChecked = false;
			SetState(GridState::INPUTS);
		}
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
		if (_grid.GetGridElement(_lastSwapGoalPos) != nullptr && _grid.GetGridElement(_lastSwapGoalPos + !_lastSwapDir) != nullptr)
			Swap(_lastSwapGoalPos, !_lastSwapDir);

		_moveChecked = false;
		SetState(GridState::INPUTS);
	}
	void MatchGrid::_FillGrid(bool createAtCenter)
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
				if (IsValidGridPosition(gridPos))
				{
					Vector2 position = GetCenterGridPosition(gridPos);
					_grid.GetGridElement(gridPos) = new MatchElement(
						*this,
						createAtCenter? Vector2::Zero() : Vector2(position.x,-WINDOW_HEIGHT - position.y),
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
		const std::string particleFileName = particlesPath + "Elements\\" + element.ToString() + "_particle";
		
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
