#include <iostream>
#include <SFML/Graphics.hpp>

#include "MatchGrid.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Types/Direction.h"

extern maoutch::Vector2 ELEMENT_SIZE;
extern maoutch::Vector2 ELEMENT_SCALE;

namespace maoutch
{
	MatchGrid::MatchGrid(const int& width, const int& height, const Vector2& position) :
		GameObject("Match Grid", 0),
		_grid(std::max(width, 5), std::max(height, 5)),
		_state(GridState::INPUTS)
	{
		Setup();
		SetPosition(position);
		UpdateElementsPosition();
	}
	MatchGrid::~MatchGrid()
	{
		_grid.Clear();
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
				{
					if (RefillBoard())
						if (!ProcessMatches()) break;

					SetState(GridState::DESTROYING);
				}
				break;

			case GridState::DESTROYING:
				if (DestroyMatched()) CollapseColumns();
				else SetState(GridState::INPUTS);
				break;

			default: break;
		}
	}

	bool MatchGrid::IsValidGridPosition(const Vector2i& gridPos) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid.GetWidth() || gridPos.y >= _grid.GetHeight()) return false;
		return true;
	}
	Vector2 MatchGrid::GetCenterGridPosition(const Vector2i& gridPos) const
	{
		const float halfWidth = (_grid.GetWidth() / 2.f) - .5f;
		const float halfHeight = (_grid.GetHeight() / 2.f) - .5f;

		Vector2 offsetPos = gridPos - Vector2(halfWidth, halfHeight);
		return offsetPos * ELEMENT_SIZE * ELEMENT_SCALE;
	}
	void MatchGrid::Setup()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				// Add element to grid
				_grid.EmplaceBack(new MatchElement(*this, gridPos, Element::Random()));
				AddChildren(_grid.GetGridElement(gridPos));

				SetupGridPos(gridPos);
			}
		}
	}
	void MatchGrid::SetupGridPos(const Vector2i& gridPos)
	{
		// Get element from grid and setup the element
		MatchElement* element = _grid.GetGridElement(gridPos);
		element->SetName((std::string)gridPos + " Element");

		// Change element until no match found with limit of 100 pass
		int count = 0;
		while (_matchFinder.MatchAt(_grid, gridPos, element->GetElement()) && count < 100)
		{
			element->SetElement(Element::Random());
			count++;
		}
	}
	void MatchGrid::Reset()
	{
		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
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

		UpdateElementsPosition();
	}
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

	bool MatchGrid::ProcessMatches()
	{
		bool matchFound = false;
		SetState(GridState::MOVING);

		_matchFinder.FindMatches(_grid);
		if (_matchFinder.matches.size() > 0) matchFound = true;
		for (std::vector<Vector2i> match : _matchFinder.matches)
		{
			for (Vector2i gridPos : match)
			{
				if (_grid.GetGridElement(gridPos) != nullptr)
					_grid.GetGridElement(gridPos)->SetIsMatched();
			}
		}

		return matchFound;
	}
	bool MatchGrid::DestroyMatched()
	{
		bool destroyed = false;

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) != nullptr && _grid.GetGridElement(gridPos)->IsMatched())
				{
					destroyed = true;
					GameObjectHandler::instance->Destroy(_grid.GetGridElement(gridPos));
					_grid.GetGridElement(gridPos) = nullptr;
				}
			}
		}

		return destroyed;
	}	bool MatchGrid::RefillBoard()
	{
		bool refilled = false;
		SetState(GridState::MOVING);

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) == nullptr)
				{
					refilled = true;
					_grid.GetGridElement(gridPos) = new MatchElement(*this, gridPos, Element::Random());
					AddChildren(_grid.GetGridElement(gridPos));
				}
			}
		}

		UpdateElementsPosition();

		return refilled;
	}

	void MatchGrid::Swap(const Vector2i gridPos, const Direction dir)
	{
		Vector2i goalPos = gridPos + dir;
		if (!IsValidGridPosition(goalPos)) return;
		if (_grid.GetGridElement(gridPos) == nullptr || _grid.GetGridElement(goalPos) == nullptr) return;
		
		_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(goalPos);
		_grid.GetGridElement(goalPos)->SetAndMoveToGridPos(gridPos);

		// Swap elements memory
		std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(goalPos));

		ProcessMatches();
	}
	void MatchGrid::CollapseColumns()
	{
		SetState(GridState::MOVING);
		
		int nullCount = 0;
		Vector2i gridPos;
		for (gridPos.x = _grid.GetWidth() - 1; gridPos.x >= 0; --gridPos.x)
		{
			for (gridPos.y = _grid.GetHeight() - 1; gridPos.y >= 0; --gridPos.y)
			{
				if (_grid.GetGridElement(gridPos) == nullptr) nullCount++;
				else if (nullCount > 0) 
				{
					Vector2i pos = { gridPos.x, gridPos.y + nullCount };
					_grid.GetGridElement(gridPos)->SetAndMoveToGridPos(pos);

					// Swap elements memory
					std::swap(_grid.GetGridElement(gridPos), _grid.GetGridElement(pos));
				}
			}
			nullCount = 0;
		}
	}
	void MatchGrid::UpdateElementsPosition()
	{
		_grid.MapAll([](MatchElement* element) { element->MoveToGridPos(); });
		SetState(GridState::MOVING);
	}

	GridState MatchGrid::GetState() const { return _state; }
	void MatchGrid::SetState(const GridState& state) { _state = state; }
}
