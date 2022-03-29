#include "MatchFinder.h"
#include "Element.h"
#include "../Objects/MatchGrid.h"
#include "../Objects/MatchElement.h"

namespace maoutch
{
	bool MatchFinder::MatchAt(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element)
	{
		// Horizontal tests
		if (gridPos.x > 1)
		{
			MatchElement* gridXLeft = grid.GetGridElement({ gridPos.x - 1, gridPos.y });
			MatchElement* gridXFarLeft = grid.GetGridElement({ gridPos.x - 2, gridPos.y });
			if (gridXLeft != nullptr && gridXFarLeft != nullptr)
				if (gridXLeft->GetElement() == element && gridXFarLeft->GetElement() == element)
					return true;
		}

		// Vertical tests
		if (gridPos.y > 1)
		{
			MatchElement* gridYTop = grid.GetGridElement({ gridPos.x, gridPos.y - 1 });
			MatchElement* gridYFarTop = grid.GetGridElement({ gridPos.x, gridPos.y - 2 });
			if (gridYTop != nullptr && gridYFarTop != nullptr)
				if (gridYTop->GetElement() == element && gridYFarTop->GetElement() == element)
					return true;
		}

		return false;
	}

	void MatchFinder::FindMatches(Grid<MatchElement*>& grid)
	{
		matches = {};

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < grid.GetHeight(); ++gridPos.y)
			for (gridPos.x = 0; gridPos.x < grid.GetWidth(); ++gridPos.x)
			{
				if (grid.GetGridElement(gridPos) == nullptr) continue;

				const Element currentElement = grid.GetGridElement(gridPos)->GetElement();

				// Horizontal tests
				std::vector<Vector2i> horizontalMatches = _CheckHorizontal(grid, gridPos, currentElement);
				if (horizontalMatches.size() > 2)
					_AddMatch(horizontalMatches);

				// Vertical tests
				std::vector<Vector2i> verticalmMatches = _CheckVertical(grid, gridPos, currentElement);
				if (verticalmMatches.size() > 2)
					_AddMatch(verticalmMatches);
			}
	}

	std::vector<Vector2i> MatchFinder::_CheckHorizontal(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element)
	{
		std::vector<Vector2i> matchedPositions{};
		for (int i = gridPos.x; i < grid.GetWidth(); ++i)
		{
			Vector2i pos = { i, gridPos.y };
			if (grid.GetGridElement(pos) == nullptr || grid.GetGridElement(pos)->GetElement() != element)
				break;

			matchedPositions.push_back(pos);
		}

		return matchedPositions;
	}
	std::vector<Vector2i> MatchFinder::_CheckVertical(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element)
	{
		std::vector<Vector2i> matchedPositions{};
		for (int i = gridPos.y; i < grid.GetHeight(); ++i)
		{
			Vector2i pos = { gridPos.x, i };
			if (grid.GetGridElement(pos) == nullptr || grid.GetGridElement(pos)->GetElement() != element)
				break;

			matchedPositions.push_back(pos);
		}

		return matchedPositions;
	}

	void MatchFinder::_AddMatch(const std::vector<Vector2i>& currentMatch)
	{
		bool foundMatch = false;
		for (std::vector<Vector2i> match : matches)
		{
			if (_VectorContains(match, currentMatch))
			{
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
			matches.push_back(currentMatch);
	}
	bool MatchFinder::_VectorContains(const std::vector<Vector2i>& vector1, std::vector<Vector2i> vector2)
	{
		return std::all_of(vector2.begin(), vector2.end(), [vector1](const Vector2i& position)
		{
				return std::find(
					vector1.begin(),
					vector1.end(),
					position
				) != vector1.end();
		});
	}
}
