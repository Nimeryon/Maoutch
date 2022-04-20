#pragma once
namespace maoutch
{
	namespace math
	{
		#define PI 3.141592653589793238462643383279502884L /* pi */
		#define RAD2DEG 180.L / PI
		#define DEG2RAD PI / 180.L

		float Clamp(const float& value, const float& min, const float& max);
		int Clamp(const int& value, const int& min, const int& max);

		// Return a blend between a and b, based on t
		float Lerp(const float a, const float b, const float t);
		// Return t based on a value between a and b
		float InverseLerp(const float a, const float b, const float value);
		// Remap value between in and out range
		float ReMap(const float inMin, const float inMax, const float outMin, const float outMax, float value);
	}
}