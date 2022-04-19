#pragma once
#include <string>

namespace maoutch
{
	class Element
	{
	public:
		enum class ElementValue
		{
			Fire,
			Water,
			Nature,
			Metal,
			Earth,
			Light,
			Dark,
			None
		};

		Element() = default;
		Element(const ElementValue& vElement);
		Element(const int& vElement);

		explicit operator bool() = delete;

		operator ElementValue() const;

		bool operator==(const Element& elem) const;
		bool operator!=(const Element& elem) const;

		// Static Methods

		[[nodiscard]] static Element Random();
		[[nodiscard]] static std::string ToString(const Element& elem);
		[[nodiscard]] static Element FromString(const std::string& string);
		[[nodiscard]] static Element GetWeakness(const Element& elem);
		[[nodiscard]] static Element GetStrength(const Element& elem);

		// Methods

		[[nodiscard]] ElementValue Value() const;
		[[nodiscard]] std::string ToString() const;
		[[nodiscard]] Element GetWeakness() const;
		[[nodiscard]] Element GetStrength() const;

	private:
		ElementValue _value;
	};
}
