#include "MatchFinder.h"
#include "../Objects/MatchGrid.h"
#include "../Objects/MatchElement.h"
#include "Data/Element.h"
#include "Data/Match.h"
#include "Data/PossibleMatch.h"

namespace maoutch
{


	MatchFinder::MatchFinder(Grid<MatchElement*>* grid) : _grid(grid) {}

	bool MatchFinder::MatchAt(const Vector2i& gridPos, const Element& element) const
	{
		// Horizontal tests
		if (gridPos.x > 1)
		{
			MatchElement* gridXLeft = _grid->GetGridElement(Vector2i(gridPos.x - 1, gridPos.y));
			MatchElement* gridXFarLeft = _grid->GetGridElement(Vector2i(gridPos.x - 2, gridPos.y));
			if (gridXLeft && gridXFarLeft)
				if (gridXLeft->GetElement() == element && gridXFarLeft->GetElement() == element)
					return true;
		}

		// Vertical tests
		if (gridPos.y > 1)
		{
			MatchElement* gridYTop = _grid->GetGridElement(Vector2i(gridPos.x, gridPos.y - 1));
			MatchElement* gridYFarTop = _grid->GetGridElement(Vector2i(gridPos.x, gridPos.y - 2));
			if (gridYTop && gridYFarTop)
				if (gridYTop->GetElement() == element && gridYFarTop->GetElement() == element)
					return true;
		}

		return false;
	}
	bool MatchFinder::IsValidPosition(const Vector2i& gridPos) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid->GetWidth() || gridPos.y >= _grid->GetHeight()) return false;
		return _grid->GetGridElement(gridPos);
	}
	bool MatchFinder::SameElement(const Vector2i& gridPos, const Element& element) const
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= _grid->GetWidth() || gridPos.y >= _grid->GetHeight()) return false;
		return IsValidPosition(gridPos) && _grid->GetGridElement(gridPos)->GetElement() == element;
	}

	bool MatchFinder::FindMatches()
	{
		matches = {};

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid->GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid->GetWidth(); ++gridPos.x)
			{
				if (!IsValidPosition(gridPos)) continue;

				const Element currentElement = _grid->GetGridElement(gridPos)->GetElement();
				
				_CheckHorizontal(gridPos, currentElement);

				_CheckVertical(gridPos, currentElement);
			}

		return !matches.empty();
	}
	bool MatchFinder::FindPossibleMatches()
	{
		possibleMatches = {};

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < _grid->GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < _grid->GetWidth(); ++gridPos.x)
			{
				if (!IsValidPosition(gridPos)) continue;

				const Element currentElement = _grid->GetGridElement(gridPos)->GetElement();

				if (!_CheckPossibleHorizontal5(gridPos, currentElement))
					if (!_CheckPossibleHorizontal4(gridPos, currentElement))
						_CheckPossibleHorizontal3(gridPos, currentElement);

				if (!_CheckPossibleVertical5(gridPos, currentElement))
					if (!_CheckPossibleVertical4(gridPos, currentElement))
						_CheckPossibleVertical3(gridPos, currentElement);
			}
		
		return !possibleMatches.empty();
	}

	void MatchFinder::_CheckHorizontal(const Vector2i& gridPos, const Element& element)
	{
		Match match;
		match.element = element;

		for (int i = gridPos.x; i < _grid->GetWidth(); ++i)
		{
			Vector2i pos = Vector2i(i, gridPos.y);
			if (!SameElement(pos, element))
				break;

			match.positions.push_back(pos);
		}

		if (match.positions.size() > 2)
			_AddMatch(match);
	}
	bool MatchFinder::_CheckPossibleHorizontal5(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  0 0 * 0 0
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(5);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  0 0 * 0 0
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y - 1), element))
		{
			possibleMatch.match.positions.reserve(5);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y - 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}
	bool MatchFinder::_CheckPossibleHorizontal4(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  * 0 * 0 0
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * 0 * 0 0
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y - 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y - 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  0 0 * 0 *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  0 0 * 0 *
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}
	bool MatchFinder::_CheckPossibleHorizontal3(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  * 0 * 0 *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * 0 * 0 *
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  0 0 * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  0 0 * * *
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 2, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  * * * 0 0
		  * * * * *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * 0 0
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y - 1), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y - 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  0 0 * X *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 3, gridPos.y), element)
			&& SameElement(Vector2i(gridPos.x - 2, gridPos.y), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 3, gridPos.y));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 2, gridPos.y));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * X * 0 0
		  * * * * *
		  * * * * *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 2, gridPos.y), element)
			&& SameElement(Vector2i(gridPos.x + 3, gridPos.y), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 2, gridPos.y));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 3, gridPos.y));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}

	void MatchFinder::_CheckVertical(const Vector2i& gridPos, const Element& element)
	{
		Match match;
		match.element = element;

		for (int i = gridPos.y; i < _grid->GetHeight(); ++i)
		{
			Vector2i pos = Vector2i(gridPos.x, i);
			if (!SameElement(pos, element))
				break;

			match.positions.push_back(pos);
		}

		if (match.positions.size() > 2)
			_AddMatch(match);
	}
	bool MatchFinder::_CheckPossibleVertical5(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * 0 *
		  * * * 0 *
		  * * X * *
		  * * * 0 *
		  * * * 0 *
		\* example  */
		if (SameElement(Vector2i(gridPos.x + 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(5);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * 0 * * *
		  * 0 * * *
		  * * X * *
		  * 0 * * *
		  * 0 * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(5);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}
	bool MatchFinder::_CheckPossibleVertical4(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * 0 *
		  * * * 0 *
		  * * X * *
		  * * * 0 *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x + 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * 0 * * *
		  * 0 * * *
		  * * X * *
		  * 0 * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * 0 *
		  * * X * *
		  * * * 0 *
		  * * * 0 *
		\* example  */
		if (SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * 0 * * *
		  * * X * *
		  * 0 * * *
		  * 0 * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}
	bool MatchFinder::_CheckPossibleVertical3(const Vector2i& gridPos, const Element& element)
	{
		PossibleMatch possibleMatch;
		possibleMatch.match.element = element;
		possibleMatch.gridPos = gridPos;

		/* example *\
		  * * * * *
		  * * * 0 *
		  * * X * *
		  * * * 0 *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(4);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * 0 * * *
		  * * X * *
		  * 0 * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * 0 *
		  * * * 0 *
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x + 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * 0 * * *
		  * 0 * * *
		  * * X * *
		  * * * * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x - 1, gridPos.y - 2), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y - 1), element)
			&& IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y - 1));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  * * * 0 *
		  * * * 0 *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x + 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x + 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x + 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::East);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * * * *
		  * * X * *
		  * 0 * * *
		  * 0 * * *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x - 1, gridPos.y))
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 1), element)
			&& SameElement(Vector2i(gridPos.x - 1, gridPos.y + 2), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 1));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x - 1, gridPos.y + 2));
			possibleMatch.direction = Direction(Direction::DirectionValue::West);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * 0 * *
		  * * 0 * *
		  * * * * *
		  * * X * *
		  * * * * *
		\* example  */
		if (SameElement(Vector2i(gridPos.x, gridPos.y - 3), element)
			&& SameElement(Vector2i(gridPos.x, gridPos.y - 2), element)
			&& IsValidPosition(Vector2i(gridPos.x, gridPos.y - 1)))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x, gridPos.y - 3));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x, gridPos.y - 2));
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.direction = Direction(Direction::DirectionValue::North);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		/* example *\
		  * * * * *
		  * * X * *
		  * * * * *
		  * * 0 * *
		  * * 0 * *
		\* example  */
		if (IsValidPosition(Vector2i(gridPos.x, gridPos.y + 1))
			&& SameElement(Vector2i(gridPos.x, gridPos.y + 2), element)
			&& SameElement(Vector2i(gridPos.x, gridPos.y + 3), element))
		{
			possibleMatch.match.positions.reserve(3);
			possibleMatch.match.positions.emplace_back(gridPos);
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x, gridPos.y + 2));
			possibleMatch.match.positions.emplace_back(Vector2i(gridPos.x, gridPos.y + 3));
			possibleMatch.direction = Direction(Direction::DirectionValue::South);

			_AddPossibleMatch(possibleMatch);
			return true;
		}

		return false;
	}

	void MatchFinder::_AddMatch(const Match& currentMatch)
	{
		bool foundMatch = false;
		for (Match& match : matches)
		{
			if (_VectorContains(match.positions, currentMatch.positions))
			{
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
			matches.push_back(currentMatch);
	}
	void MatchFinder::_AddPossibleMatch(const PossibleMatch& currentPossibleMatch)
	{
		bool foundMatch = false;
		for (PossibleMatch& possibleMatch : possibleMatches)
		{
			if (_VectorContains(possibleMatch.match.positions, currentPossibleMatch.match.positions))
			{
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
			possibleMatches.push_back(currentPossibleMatch);
	}

	bool MatchFinder::_VectorContains(const std::vector<Vector2i>& vector1, std::vector<Vector2i> vector2) const
	{
		return std::all_of(vector2.begin(), vector2.end(), [vector1](const Vector2i& position)
		{
			return std::find(vector1.begin(), vector1.end(), position) != vector1.end();
		});
	}
}
