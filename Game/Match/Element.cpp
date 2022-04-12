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
		const int elem = random::Int(0, (int)ElementValue::None);
		return Element((ElementValue)elem);
	}
	std::string Element::ToString(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::Fire: return "Fire";
			case ElementValue::Water: return "Water";
			case ElementValue::Nature: return "Nature";
			case ElementValue::Metal: return "Metal";
			case ElementValue::Earth: return "Earth";
			case ElementValue::Light: return "Light";
			case ElementValue::Dark: return "Dark";
			default: return "None";
		}
	}
	Element Element::FromString(const std::string& string)
	{
		std::string value = string::ToLower(string);

		if (value == "fire") return ElementValue::Fire;
		if (value == "water") return ElementValue::Water;
		if (value == "nature") return ElementValue::Nature;
		if (value == "metal") return ElementValue::Metal;
		if (value == "earth") return ElementValue::Earth;
		if (value == "light") return ElementValue::Light;
		if (value == "dark") return ElementValue::Dark;
		return ElementValue::None;
	}
	Element Element::GetWeakness(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::Fire: return ElementValue::Water;
			case ElementValue::Water: return ElementValue::Earth;
			case ElementValue::Nature: return ElementValue::Metal;
			case ElementValue::Metal: return ElementValue::Fire;
			case ElementValue::Earth: return ElementValue::Nature;
			case ElementValue::Light: return ElementValue::Dark;
			case ElementValue::Dark: return ElementValue::Light;
			default: return ElementValue::None;
		}
	}
	Element Element::GetStrength(const Element& elem)
	{
		switch (elem)
		{
			case ElementValue::Fire: return ElementValue::Metal;
			case ElementValue::Water: return ElementValue::Fire;
			case ElementValue::Nature: return ElementValue::Earth;
			case ElementValue::Metal: return ElementValue::Nature;
			case ElementValue::Earth: return ElementValue::Water;
			case ElementValue::Light: return ElementValue::None;
			case ElementValue::Dark: return ElementValue::None;
			default: return ElementValue::None;
		}
	}

	// Methods

	Element::ElementValue Element::Value() const { return _value; }
	std::string Element::ToString() const { return ToString(*this); }
	Element Element::GetWeakness() const { return GetWeakness(*this); }
	Element Element::GetStrength() const { return GetStrength(*this); }
}
