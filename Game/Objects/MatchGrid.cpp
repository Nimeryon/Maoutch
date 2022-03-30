#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "MatchGrid.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Types/Direction.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern maoutch::Vector2 ELEMENT_SIZE;
extern maoutch::Vector2 ELEMENT_SCALE;

namespace maoutch
{
	MatchGrid::MatchGrid(const std::string& fileName, const Vector2& position) :
		GameObject("Match Grid", 0),
		_emptyPositions({}),
		_state(GridState::INPUTS),
		_moveChecked(false),
		_processAfterMoving(false),
		_lastSwapDir(Direction::DirectionValue::NONE),
		_destroyTimer(.33f, &MatchGrid::DestroyMatched, this),
		_collapseTimer(.2f, &MatchGrid::CollapseColumns, this),
		_refillTimer(.2f, &MatchGrid::RefillBoard, this)
	{
		Setup(fileName);
		SetPosition(position);
		UpdateElementsPosition();
	}
	
	void MatchGrid::FixedUpdate(float fixedDt)
	{
		switch (_state)
		{
			case GridState::MOVING:
				if (!std::any_of(
					_grid.array.begin(),
					_grid.array.end(),
					[](const MatchElement* element) { return element != nullptr && element->IsMoving(); }
				))
					if (_processAfterMoving) ProcessMatches();
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

		// Setup empty grid
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
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < gridSize.y; ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < gridSize.x; ++gridPos.x)
			{
				// Add element to grid
				if (IsValidGridPosition(gridPos))
				{
					_grid.EmplaceBack(new MatchElement(*this, gridPos, Element::Random()));
					AddChildren(_grid.GetGridElement(gridPos));

					SetupGridPos(gridPos);
				}
				else _grid.EmplaceBack(nullptr);
			}
		}
	}
	void MatchGrid::SetupGridPos(const Vector2i& gridPos)
	{
		// Get element from grid and setup the element
		MatchElement* matchElem = _grid.GetGridElement(gridPos);
		matchElem->SetName((std::string)gridPos + " Element");

		Element element = matchElem->GetElement();

		// Change element until no match found with limit of 100 pass
		int count = 0;
		while (_matchFinder.MatchAt(_grid, gridPos, element) && count < 100)
		{
			element = Element::Random();
			count++;
		}

		matchElem->SetElement(element);
	}

	bool MatchGrid::IsValidGridPosition(const Vector2i& gridPos) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid.GetWidth() || gridPos.y >= _grid.GetHeight()) return false;
		return std::find(_emptyPositions.begin(), _emptyPositions.end(), gridPos) == _emptyPositions.end();
	}
	bool MatchGrid::ProcessMatches()
	{
		bool matchFound = false;
		if (_processAfterMoving) _processAfterMoving = false;

		_matchFinder.FindMatches(_grid);
		if (!_matchFinder.matches.empty()) matchFound = true;
		for (std::vector<Vector2i> match : _matchFinder.matches)
		{
			for (Vector2i gridPos : match)
			{
				if (_grid.GetGridElement(gridPos) != nullptr)
					_grid.GetGridElement(gridPos)->SetIsMatched();
			}
		}

		_destroyTimer.Start();

		return matchFound;
	}

	void MatchGrid::Reset()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos))
				{
					MatchElement* element = _grid.GetGridElement(gridPos);
					if (element != nullptr)
					{
						element->SetElement(Element::Random());
					}
					else
					{
						_grid.GetGridElement(gridPos) = new MatchElement(*this, gridPos, Element::Random());
						AddChildren(_grid.GetGridElement(gridPos));
					}

					SetupGridPos(gridPos);
				}
			}
		}

		UpdateElementsPosition();
	}
	void MatchGrid::DestroyMatched()
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
						GameObjectHandler::instance->Destroy(_grid.GetGridElement(gridPos));
						_grid.GetGridElement(gridPos) = nullptr;
					}
			}
		}

		_moveChecked = true;

		if (matched) _collapseTimer.Start();
		else SwapBack();
	}
	void MatchGrid::CollapseColumns()
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
	void MatchGrid::RefillBoard()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos) == nullptr)
				{
					_grid.GetGridElement(gridPos) = new MatchElement(*this, gridPos, Element::Random());
					AddChildren(_grid.GetGridElement(gridPos));
				}
			}
		}

		UpdateElementsPosition();
		AfterRefill();
	}
	void MatchGrid::AfterRefill()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (IsValidGridPosition(gridPos) && _grid.GetGridElement(gridPos) != nullptr)
				{
					if (_matchFinder.MatchAt(_grid, gridPos, _grid.GetGridElement(gridPos)->GetElement()))
					{
						_processAfterMoving = true;
						return;
					}
				}
			}
		}

		SetState(GridState::INPUTS);
		_moveChecked = false;
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
		
		_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(goalPos);
		_grid.GetGridElement(goalPos)->SetAndMoveToGridPos(gridPos);

		// Swap elements memory
		std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(goalPos));

		_lastSwapGoalPos = goalPos;
		_lastSwapDir = dir;

		if (!_moveChecked) ProcessMatches();
	}
	void MatchGrid::SwapBack()
	{
		if (_grid.GetGridElement(_lastSwapGoalPos) != nullptr && _grid.GetGridElement(_lastSwapGoalPos + !_lastSwapDir) != nullptr)
			Swap(_lastSwapGoalPos, !_lastSwapDir);

		SetState(GridState::INPUTS);
		_moveChecked = false;
	}
	void MatchGrid::UpdateElementsPosition()
	{
		_grid.MapAll([](MatchElement* element) { element->MoveToGridPos(); });
	}

	Vector2 MatchGrid::GetCenterGridPosition(const Vector2i& gridPos) const
	{
		const float halfWidth = (_grid.GetWidth() / 2.f) - .5f;
		const float halfHeight = (_grid.GetHeight() / 2.f) - .5f;

		Vector2 offsetPos = gridPos - Vector2(halfWidth, halfHeight);
		return offsetPos * ELEMENT_SIZE * ELEMENT_SCALE;
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
}
