#pragma once
#include <SFML/Graphics/Color.hpp>

#include "Vector2.h"
#include "../Tools/Colors.h"
#include "../Tools/Math.h"

namespace maoutch
{
	template<typename T>
	struct LerpableValue
	{
		LerpableValue(const T& startValue, const T& endValue) : startValue(startValue), endValue(endValue) {}
		LerpableValue(const T& value) : startValue(value), endValue(value) {}

		// Return value between startValue and endValue base on t, if values are the same return startValue
		[[nodiscard]] T GetValue(const float& t = 1) const
		{
			if (startValue == endValue) return startValue;
			return math::Lerp(startValue, endValue, t);
		}
		
		T startValue;
		T endValue;
	};

	template<>
	struct LerpableValue<sf::Color>
	{
		LerpableValue(const sf::Color& startColor, const sf::Color& endColor) : startColor(startColor), endColor(endColor) {}
		LerpableValue(const sf::Color& color) : startColor(color), endColor(color) {}

		// Return color value between startColor and endColor base on t, if values are the same return startColor
		[[nodiscard]] sf::Color GetValue(const float& t = 1) const
		{
			if (startColor == endColor) return startColor;
			return colors::LerpRGB(startColor, endColor, t);
		}
		
		sf::Color startColor;
		sf::Color endColor;
	};

	template<>
	struct LerpableValue<Vector2>
	{
		LerpableValue(const Vector2& startVector, const Vector2& endVector) : startVector(startVector), endVector(endVector) {}
		LerpableValue(const Vector2& vector) : startVector(vector), endVector(vector) {}

		// Return color value between startColor and endColor base on t, if values are the same return startColor
		[[nodiscard]] Vector2 GetValue(const float& t = 1) const
		{
			if (startVector == endVector) return startVector;
			return Vector2::Lerp(startVector, endVector, t).Normalized();
		}

		Vector2 startVector;
		Vector2 endVector;
	};
}