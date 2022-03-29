#pragma once
#include <string>

#include "../Types/Vector2i.h"

namespace maoutch
{
	class Direction
	{
	public:
		enum class DirectionValue
		{
			NORTH,
			EAST,
			SOUTH,
			WEST,
			NONE
		};

		Direction() = default;
		Direction(const DirectionValue& vDirection);

		explicit operator bool() = delete;

		operator DirectionValue() const;
		operator Vector2() const;
		operator Vector2i() const;

		Direction operator!() const;
		bool operator==(const Direction& dir) const;
		bool operator!=(const Direction& dir) const;

		// Static Methods

		[[nodiscard]] static std::string ToString(const Direction& dir);
		[[nodiscard]] static Direction FromString(const std::string& string);
		[[nodiscard]] static Direction GetOpposite(const Direction& dir);
		[[nodiscard]] static Direction GetDirection(const Vector2& dir);
		[[nodiscard]] static Direction GetDirection(const float& angle);

		// Methods

		[[nodiscard]] std::string ToString() const;
		[[nodiscard]] Direction GetOpposite() const;

	private:
		DirectionValue _value;
	};
}
