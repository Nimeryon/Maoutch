#pragma once
#include "../../../Types/Vector2i.h"
#include "../../../Types/Direction.h"
#include "Match.h"

namespace maoutch
{
	struct PossibleMatch
	{
		PossibleMatch();

		Match match;
		Vector2i gridPos;
		Direction direction;
	};
}
