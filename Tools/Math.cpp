#include <algorithm>

#include "Math.h"

namespace maoutch
{
	namespace math
	{
		float Clamp(const float& value, const float& min, const float& max)
		{
			return std::min(max, std::max(value, min));
		}
		int Clamp(const int& value, const int& min, const int& max)
		{
			return std::min(max, std::max(value, min));
		}
		
		float Lerp(const float a, const float b, const float t)
		{
			return a + (b - a) * t;
		}
		float InverseLerp(const float a, const float b, const float value)
		{
			return (value - a) / (b - a);
		}
		float ReMap(const float inMin, const float inMax, const float outMin, const float outMax, float value)
		{
			float t = InverseLerp(inMin, inMax, value);
			return Lerp(outMin, outMax, t);
		}

	}
}
