#pragma once
#include <vector>

#include "../../Types/Vector2i.h"
#include "../../Types/Grid.h"

namespace maoutch
{
	class MatchElement;
	class Element;
	
	class MatchFinder
	{
	public:
		bool MatchAt(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element);

		// Find matches in grid, Return number of matches
		void FindMatches(Grid<MatchElement*>& grid);

		std::vector<std::vector<Vector2i>> matches{};

	private:
		std::vector<Vector2i> _CheckHorizontal(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element);
		std::vector<Vector2i> _CheckVertical(Grid<MatchElement*>& grid, const Vector2i& gridPos, const Element& element);

		void _AddMatch(const std::vector<Vector2i>& currentMatch);
		bool _VectorContains(const std::vector<Vector2i>& vector1, std::vector<Vector2i> vector2);
	};
}
