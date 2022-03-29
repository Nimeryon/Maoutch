#pragma warning( disable : 4244 )
#pragma once

namespace maoutch
{
	namespace random
	{
		// Generate random float value between 0.f and 1.f
		const float Float();
		// Generate random float value between [min, max[
		const float Float(const float& min, const float& max);

		// Generate random boolean value
		const bool Bool();

		// Generate random int value between [min, max[
		const int Int(const int& min, const int& max);
	}
}
