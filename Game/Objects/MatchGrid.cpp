#include <iostream>
#include <SFML/Graphics.hpp>

#include "MatchGrid.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Types/Direction.h"
#include "../../Tools/Random.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern maoutch::Vector2 ELEMENT_SIZE;
extern maoutch::Vector2 ELEMENT_SCALE;

namespace maoutch
{
	MatchGrid::MatchGrid(const int& width, const int& height, const Vector2& position) :
		GameObject("Match Grid", 0),
		_velocity(Vector2(random::Float(-50, 50), random::Float(-50, 50))),
		_grid(std::max(width, 5), std::max(height, 5)),
		_state(GridState::INPUTS),
		_destroyTimer(.5f, &MatchGrid::DestroyMatched, this),
		_collapseTimer(.33f, &MatchGrid::CollapseColumns, this),
		_refillTimer(.33f, &MatchGrid::RefillBoard, this)
	{
		Setup();
		SetPosition(position);
		UpdateElementsPosition();
	}
	MatchGrid::~MatchGrid()
	{
		_grid.Clear();
	}

	void MatchGrid::Update(float dt)
	{
		if (GetPosition().x < 0 || GetPosition().x >= WINDOW_WIDTH)
			_velocity.x *= -1;

		if (GetPosition().y < 0 || GetPosition().y >= WINDOW_HEIGHT)
			_velocity.y *= -1;

		Move(_velocity * dt);
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
					SetState(GridState::INPUTS);
				}
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

		if (matchFound)
			_destroyTimer.Start();

		return matchFound;
	}
	void MatchGrid::DestroyMatched()
	{
		std::cout << "Destroy\n";

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) != nullptr && _grid.GetGridElement(gridPos)->IsMatched())
				{
					GameObjectHandler::instance->Destroy(_grid.GetGridElement(gridPos));
					_grid.GetGridElement(gridPos) = nullptr;
				}
			}
		}

		_collapseTimer.Start();
	}
	void MatchGrid::RefillBoard()
	{
		std::cout << "Refill\n";

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) == nullptr)
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
		std::cout << "After Refill\n";

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < _grid.GetWidth(); ++gridPos.x)
			{
				if (_grid.GetGridElement(gridPos) != nullptr)
				{
					if (_matchFinder.MatchAt(_grid, gridPos, _grid.GetGridElement(gridPos)->GetElement()))
					{
						ProcessMatches();
						return;
					}
				}
			}
		}
	}
	void MatchGrid::Swap(const Vector2i gridPos, const Direction dir)
	{
		SetState(GridState::MOVING);

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
		std::cout << "Collapse\n";
		
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

		_refillTimer.Start();
	}
	void MatchGrid::UpdateElementsPosition()
	{
		_grid.MapAll([](MatchElement* element) { element->MoveToGridPos(); });
	}

	GridState MatchGrid::GetState() const { return _state; }
	void MatchGrid::SetState(const GridState& state) { _state = state; }
}
