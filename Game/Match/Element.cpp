#include "Element.h"
#include "../../Tools/String.h"
#include "../../Tools/Random.h"

namespace maoutch
{
	Element::Element(const ElementValue& vElement) : _value(vElement) {}
	Element::Element(const int& vElement) : _value((ElementValue)vElement) {}

	Element::operator ElementValue() const { return _value; }

	bool Element::operator==(const Element& dir) const { return _value == dir._value; }
	bool Element::operator!=(const Element& dir) const { return _value != dir._value; }

	// Static Methods

	Element Element::Random()
	{
		const int elem = random::Int(0, (int)ElementValue::NONE);
		return Element((ElementValue)elem);
	}
	std::string Element::ToString(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::FIRE: return "Fire";
			case ElementValue::WATER: return "Water";
			case ElementValue::NATURE: return "Nature";
			case ElementValue::METAL: return "Metal";
			case ElementValue::EARTH: return "Earth";
			case ElementValue::LIGHT: return "Light";
			case ElementValue::DARK: return "Dark";
			default: return "None";
		}
	}
	Element Element::FromString(const std::string& string)
	{
		std::string value = string::ToLower(string);

		if (value == "fire") return ElementValue::FIRE;
		if (value == "water") return ElementValue::WATER;
		if (value == "nature") return ElementValue::NATURE;
		if (value == "metal") return ElementValue::METAL;
		if (value == "earth") return ElementValue::EARTH;
		if (value == "light") return ElementValue::LIGHT;
		if (value == "dark") return ElementValue::DARK;
		return ElementValue::NONE;
	}
	Element Element::GetWeakness(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::FIRE: return ElementValue::WATER;
			case ElementValue::WATER: return ElementValue::EARTH;
			case ElementValue::NATURE: return ElementValue::METAL;
			case ElementValue::METAL: return ElementValue::FIRE;
			case ElementValue::EARTH: return ElementValue::NATURE;
			case ElementValue::LIGHT: return ElementValue::DARK;
			case ElementValue::DARK: return ElementValue::LIGHT;
			default: return ElementValue::NONE;
		}
	}
	Element Element::GetStrength(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::FIRE: return ElementValue::METAL;
			case ElementValue::WATER: return ElementValue::FIRE;
			case ElementValue::NATURE: return ElementValue::EARTH;
			case ElementValue::METAL: return ElementValue::NATURE;
			case ElementValue::EARTH: return ElementValue::WATER;
			case ElementValue::LIGHT: return ElementValue::NONE;
			case ElementValue::DARK: return ElementValue::NONE;
			default: return ElementValue::NONE;
		}
	}

	// Methods

	Element::ElementValue Element::Value() const { return _value; }
	std::string Element::ToString() const { return ToString(*this); }
	Element Element::GetWeakness() const { return GetWeakness(*this); }
	Element Element::GetStrength() const { return GetStrength(*this); }
}
