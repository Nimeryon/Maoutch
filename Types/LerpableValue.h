#pragma once
#include <SFML/Graphics/Color.hpp>

#include "Vector2.h"
#include "../Tools/Colors.h"
#include "../Tools/Math.h"
#include "../Tools/Easing.h"

namespace maoutch
{
	template<typename T>
	struct LerpableValue
	{
		LerpableValue(const T& startValue, const T& endValue, const easing::EaseType& easeType = easing::EaseType::None) : startValue(startValue), endValue(endValue), easeType(easeType) {}
		LerpableValue(const T& value, const easing::EaseType& easeType = easing::EaseType::None) : startValue(value), endValue(value), easeType(easeType) {}

		// Return value between startValue and endValue base on t, if values are the same return startValue
		[[nodiscard]] T GetValue(const float& t = 1) const
		{
			if (startValue == endValue) return startValue;
			return math::Lerp(startValue, endValue, Ease(easeType, t));
		}
		
		T startValue;
		T endValue;
		easing::EaseType easeType;
	};

	template<>
	struct LerpableValue<sf::Color>
	{
		LerpableValue(const sf::Color& startColor, const sf::Color& endColor, const easing::EaseType& easeType = easing::EaseType::None) : startColor(startColor), endColor(endColor), easeType(easeType) {}
		LerpableValue(const sf::Color& color, const easing::EaseType& easeType = easing::EaseType::None) : startColor(color), endColor(color), easeType(easeType) {}

		// Return color value between startColor and endColor base on t, if values are the same return startColor
		[[nodiscard]] sf::Color GetValue(const float& t = 1) const
		{
			if (startColor == endColor) return startColor;
			return colors::LerpRGB(startColor, endColor, t, easeType);
		}
		
		sf::Color startColor;
		sf::Color endColor;
		easing::EaseType easeType;
	};

	template<>
	struct LerpableValue<Vector2>
	{
		LerpableValue(const Vector2& startVector, const Vector2& endVector, const easing::EaseType& easeType = easing::EaseType::None) : startVector(startVector), endVector(endVector), easeType(easeType) {}
		LerpableValue(const Vector2& vector, const easing::EaseType& easeType = easing::EaseType::None) : startVector(vector), endVector(vector), easeType(easeType) {}

		// Return color value between startColor and endColor base on t, if values are the same return startColor
		[[nodiscard]] Vector2 GetValue(const float& t = 1) const
		{
			if (startVector == endVector) return startVector;
			return Vector2::Lerp(startVector, endVector, t, easeType);
		}

		Vector2 startVector;
		Vector2 endVector;
		easing::EaseType easeType;
	};
}