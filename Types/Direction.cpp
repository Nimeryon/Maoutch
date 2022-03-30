#include "Direction.h"
#include "../Tools/String.h"
#include "../Tools/Math.h"

namespace maoutch
{
	Direction::Direction(const DirectionValue& vDirection) : _value(vDirection) {}
	
	Direction::operator DirectionValue() const { return _value; }
	Direction::operator Vector2() const
	{
		switch (_value)
		{
			case DirectionValue::NORTH: return Vector2::Up();
			case DirectionValue::EAST: return Vector2::Right();
			case DirectionValue::SOUTH: return Vector2::Down();
			case DirectionValue::WEST: return Vector2::Left();
			default: return Vector2::Zero();
		}
	}
	Direction::operator Vector2i() const
	{
		switch (_value)
		{
			case DirectionValue::NORTH: return Vector2i::Up();
			case DirectionValue::EAST: return Vector2i::Right();
			case DirectionValue::SOUTH: return Vector2i::Down();
			case DirectionValue::WEST: return Vector2i::Left();
			default: return Vector2i::Zero();
		}
	}

	Direction Direction::operator!() const
	{
		switch (_value)
		{
			case DirectionValue::NORTH: return DirectionValue::SOUTH;
			case DirectionValue::EAST: return DirectionValue::WEST;
			case DirectionValue::SOUTH: return DirectionValue::NORTH;
			case DirectionValue::WEST: return DirectionValue::EAST;
			default: return DirectionValue::NONE;
		}
	}
	bool Direction::operator==(const Direction& dir) const { return _value == dir._value; }
	bool Direction::operator!=(const Direction& dir) const { return _value != dir._value; }

	// Static Methods

	std::string Direction::ToString(const Direction& dir)
	{
		switch (dir)
		{
			case DirectionValue::NORTH: return "North";
			case DirectionValue::EAST: return "East";
			case DirectionValue::SOUTH: return "South";
			case DirectionValue::WEST: return "West";
			default: return "None";
		}
	}
	Direction Direction::FromString(const std::string& string)
	{
		std::string value = string::ToLower(string);

		if (value == "north") return DirectionValue::NORTH;
		if (value == "east") return DirectionValue::EAST;
		if (value == "south") return DirectionValue::SOUTH;
		if (value == "west") return DirectionValue::WEST;
		return DirectionValue::NONE;
	}
	Direction Direction::GetOpposite(const Direction& dir) { return !dir; }
	Direction Direction::GetDirection(const Vector2& dir)
	{
		if (dir == Vector2::Up()) return DirectionValue::NORTH;
		if (dir == Vector2::Right()) return DirectionValue::EAST;
		if (dir == Vector2::Down()) return DirectionValue::SOUTH;
		if (dir == Vector2::Left()) return DirectionValue::WEST;
		return DirectionValue::NONE;
	}
	Direction Direction::GetDirection(const float& angle, const float& offset)
	{
		const int offsetAngle = ((int)(angle - offset) % 360 + 360) % 360;

		if (offsetAngle > 45 && offsetAngle <= 135) return DirectionValue::NORTH;
		if (offsetAngle > 135 && offsetAngle <= 225) return DirectionValue::EAST;
		if (offsetAngle > 225 && offsetAngle <= 315) return DirectionValue::SOUTH;
		return DirectionValue::WEST;
	}

	// Methods

	std::string Direction::ToString() const { return ToString(*this); }
	Direction Direction::GetOpposite() const { return GetOpposite(*this); }
}
