#pragma once
#include <vector>

#include "../../Types/Vector2i.h"
#include "../../Types/Grid.h"
#include "../../Types/Direction.h"
#include "Element.h"

namespace maoutch
{
	class MatchElement;

	struct Match
	{
		std::vector<Vector2i> positions;
		Element element;
	};

	struct PossibleMatch
	{
		Match match;
		Vector2i gridPos;
		Direction direction;
	};

	class MatchFinder
	{
	public:
		MatchFinder(Grid<MatchElement*>* grid);

		bool MatchAt(const Vector2i& gridPos, const Element& element) const;
		bool IsValidPosition(const Vector2i& gridPos) const;
		bool SameElement(const Vector2i& gridPos, const Element& element) const;

		bool FindMatches();
		bool FindPossibleMatches();

		std::vector<Match> matches {};
		std::vector<PossibleMatch> possibleMatches {};

	private:
		Grid<MatchElement*>* _grid;

		void _CheckHorizontal(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleHorizontal5(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleHorizontal4(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleHorizontal3(const Vector2i& gridPos, const Element& element);

		void _CheckVertical(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleVertical5(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleVertical4(const Vector2i& gridPos, const Element& element);
		bool _CheckPossibleVertical3(const Vector2i& gridPos, const Element& element);

		void _AddMatch(const Match& currentMatch);
		void _AddPossibleMatch(const PossibleMatch& currentPossibleMatch);
		
		bool _VectorContains(const std::vector<Vector2i>& vector1, std::vector<Vector2i> vector2) const;
	};
}
