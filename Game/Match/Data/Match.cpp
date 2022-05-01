#include "Match.h"
#include "Element.h"

namespace maoutch
{
	Match::Match() :
		positions({}),
		element(Element::ElementValue::None)
	{}

	Vector2 Match::GetAveragePosition()
	{
		Vector2 avgPos;
		for (Vector2i& position : positions)
			avgPos += position;

		return avgPos / positions.size();
	}
}