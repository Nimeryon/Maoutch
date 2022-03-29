#include <random>
#include <time.h>

#include "Random.h"

namespace maoutch
{
	namespace random
	{
		const float Float()
		{
			return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		}
		const float Float(const float& min, const float& max)
		{
			if (min == max) return min;

			float rdm = (Float() * (max - min)) + min;
			if (rdm == max) return max - .00001f;
			return rdm;
		}

		const bool Bool() {	return Float() >= .5f; }

		const int Int(const int& min, const int& max)
		{
			if (min == max) return min;

			int rdm = static_cast<int>(Float(min, max));
			if (rdm == max) return max - 1;
			return rdm;
		}
	}
}