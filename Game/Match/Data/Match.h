#pragma once
#include "../../../Types/Vector2i.h"
#include "Element.h"

namespace maoutch
{
	struct Match
	{
		Match();

		std::vector<Vector2i> positions;
		Element element;
	};
}