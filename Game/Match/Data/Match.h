#pragma once
#include "../../../Types/Vector2i.h"
#include "Element.h"

namespace maoutch
{
	struct Match
	{
		Match();

		Vector2 GetAveragePosition();

		std::vector<Vector2i> positions;
		Element element;
	};
}