#include "Direction.h"
#include "../Tools/String.h"

namespace maoutch
{
	Direction::Direction(const DirectionValue& vDirection) : _value(vDirection) {}
	
	Direction::operator DirectionValue() const { return _value; }
	Direction::operator Vector2() const
	{
		switch (_value)
		{
			case DirectionValue::North: return Vector2::Up();
			case DirectionValue::East: return Vector2::Right();
			case DirectionValue::South: return Vector2::Down();
			case DirectionValue::West: return Vector2::Left();
			default: return Vector2::Zero();
		}
	}
	Direction::operator Vector2i() const
	{
		switch (_value)
		{
			case DirectionValue::North: return Vector2i::Up();
			case DirectionValue::East: return Vector2i::Right();
			case DirectionValue::South: return Vector2i::Down();
			case DirectionValue::West: return Vector2i::Left();
			default: return Vector2i::Zero();
		}
	}

	Direction Direction::operator!() const
	{
		switch (_value)
		{
			case DirectionValue::North: return DirectionValue::South;
			case DirectionValue::East: return DirectionValue::West;
			case DirectionValue::South: return DirectionValue::North;
			case DirectionValue::West: return DirectionValue::East;
			default: return DirectionValue::None;
		}
	}
	bool Direction::operator==(const Direction& dir) const { return _value == dir._value; }
	bool Direction::operator!=(const Direction& dir) const { return _value != dir._value; }

	// Static Methods

	std::string Direction::ToString(const Direction& dir)
	{
		switch (dir)
		{
			case DirectionValue::North: return "North";
			case DirectionValue::East: return "East";
			case DirectionValue::South: return "South";
			case DirectionValue::West: return "West";
			default: return "None";
		}
	}
	Direction Direction::FromString(const std::string& string)
	{
		std::string value = string::ToLower(string);

		if (value == "north") return DirectionValue::North;
		if (value == "east") return DirectionValue::East;
		if (value == "south") return DirectionValue::South;
		if (value == "west") return DirectionValue::West;
		return DirectionValue::None;
	}
	Direction Direction::GetOpposite(const Direction& dir) { return !dir; }
	Direction Direction::GetDirection(const Vector2& dir)
	{
		if (dir == Vector2::Up()) return DirectionValue::North;
		if (dir == Vector2::Right()) return DirectionValue::East;
		if (dir == Vector2::Down()) return DirectionValue::South;
		if (dir == Vector2::Left()) return DirectionValue::West;
		return DirectionValue::None;
	}
	Direction Direction::GetDirection(const float& angle, const float& offset)
	{
		const int offsetAngle = ((int)(angle - offset) % 360 + 360) % 360;

		if (offsetAngle > 45 && offsetAngle <= 135) return DirectionValue::North;
		if (offsetAngle > 135 && offsetAngle <= 225) return DirectionValue::East;
		if (offsetAngle > 225 && offsetAngle <= 315) return DirectionValue::South;
		return DirectionValue::West;
	}

	// Methods

	std::string Direction::ToString() const { return ToString(*this); }
	Direction Direction::GetOpposite() const { return GetOpposite(*this); }
}
