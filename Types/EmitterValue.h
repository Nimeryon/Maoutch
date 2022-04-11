#pragma once
#include <SFML/Graphics/Color.hpp>

#include "LerpableValue.h"
#include "Vector2.h"
#include "../../Tools/Random.h"
#include "../../Tools/Colors.h"
#include "../../Tools/String.h"
#include "../../Tools/Easing.h"

namespace maoutch
{
	/*
	Type = 0: Fixed
	Type = 1: Random
	Type = 2: Lerp
	Type = 3: RandomLerp
	 */
	template<typename T>
	struct EmitterValue
	{
		EmitterValue(const T& minXValue, const T& maxXValue, const T& minYValue, const T& maxYValue, const int& type, const easing::EaseType& easeType = easing::EaseType::None) :
			type(type),
			minXValue(minXValue),
			minYValue(minYValue),
			maxXValue(maxXValue),
			maxYValue(maxYValue),
			easeType(easeType) {}
		EmitterValue(const T& minValue, const T& maxValue, const int& type, const easing::EaseType& easeType = easing::EaseType::None) :
			type(type),
			minXValue(minValue),
			minYValue(minValue),
			maxXValue(maxValue),
			maxYValue(maxValue),
			easeType(easeType) {}
		EmitterValue(const T& value, const easing::EaseType& easeType = easing::EaseType::None) :
			type(0),
			minXValue(value),
			minYValue(value),
			maxXValue(value),
			maxYValue(value),
			easeType(easeType) {}

		[[nodiscard]] LerpableValue<T> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<T>(minXValue, easeType);

				case 1:
					return LerpableValue<T>(random::Float(minXValue, maxXValue), easeType);

				case 2:
					return LerpableValue<T>(minXValue, maxXValue, easeType);

				default:
					return LerpableValue<T>(random::Float(minXValue, maxXValue), random::Float(minYValue, maxYValue), easeType);
			}
		}
		void FromString(std::string value)
		{
			const auto values = string::Split(value, ',');
			type = std::atoi(&values[0][0]);
			minXValue = std::atof(&values[1][0]);
			maxXValue = std::atof(&values[2][0]);
			maxYValue = std::atof(&values[3][0]);
			maxYValue = std::atof(&values[4][0]);
			easeType = (easing::EaseType)std::atoi(&values[5][0]);
		}

		int type;
		T minXValue;
		T minYValue;
		T maxXValue;
		T maxYValue;
		easing::EaseType easeType;
	};

	template<>
	struct EmitterValue<sf::Color>
	{
		EmitterValue(const sf::Color& startColor, const sf::Color& endColor, const int& type, const easing::EaseType& easeType = easing::EaseType::None) :
			type(type),
			startColor(startColor),
			endColor(endColor),
			easeType(easeType) {}
		EmitterValue(const sf::Color& color, const easing::EaseType& easeType = easing::EaseType::None) :
			type(0),
			startColor(color),
			endColor(color),
			easeType(easeType) {}
		EmitterValue(const easing::EaseType& easeType = easing::EaseType::None) :
			type(1),
			easeType(easeType) {}

		[[nodiscard]] LerpableValue<sf::Color> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<sf::Color>(startColor, easeType);

				case 1:
				{
					float color1[4] = {
						random::Float(),
						random::Float(),
						random::Float(),
						random::Float()
					};
					float color2[4] = {
						random::Float(),
						random::Float(),
						random::Float(),
						random::Float()
					};

					return LerpableValue<sf::Color>(colors::FromFloat4(color1), colors::FromFloat4(color2), easeType);
				}

				default:
					return LerpableValue<sf::Color>(startColor, endColor, easeType);
			}
		}
		void FromString(std::string value)
		{
			const auto values = string::Split(value, ',');
			type = std::atoi(&values[0][0]);
			startColor = colors::FromString(values[1]);
			endColor = colors::FromString(values[2]);
			easeType = (easing::EaseType)std::atoi(&values[3][0]);
		}

		int type;
		sf::Color startColor;
		sf::Color endColor;
		easing::EaseType easeType;
	};

	template<>
	struct EmitterValue<Vector2>
	{
		EmitterValue(const Vector2& xVector, const Vector2& yVector, const int& type, const easing::EaseType& easeType = easing::EaseType::None) :
			type(type),
			xVector(xVector),
			yVector(yVector),
			easeType(easeType) {}
		EmitterValue(const Vector2& vector, const easing::EaseType& easeType = easing::EaseType::None) :
			type(0),
			xVector(vector),
			yVector(vector),
			easeType(easeType) {}
		EmitterValue(const easing::EaseType& easeType = easing::EaseType::None) :
			type(1),
			easeType(easeType) {}

		[[nodiscard]] LerpableValue<Vector2> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<Vector2>(xVector, easeType);

				case 1:
					return LerpableValue<Vector2>(Vector2(random::Float(xVector.x, xVector.y), random::Float(yVector.x, yVector.y)).Normalized(), easeType);

				default:
					return LerpableValue<Vector2>(xVector, yVector, easeType);
			}
		}
		void FromString(std::string value)
		{
			const auto values = string::Split(value, ',');
			type = std::atoi(&values[0][0]);
			xVector = Vector2::FromString(string::Replace(values[1], "\"", ""));
			yVector = Vector2::FromString(string::Replace(values[2], "\"", ""));
			easeType = (easing::EaseType)std::atoi(&values[3][0]);
		}

		int type;
		Vector2 xVector;
		Vector2 yVector;
		easing::EaseType easeType;
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<T>& e)
	{
		os << e.type << "," << e.minXValue << "," << e.maxXValue << "," << e.minYValue << "," << e.maxYValue << "," << (int)e.easeType;
		return os;
	}
	template<>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<sf::Color>& e)
	{
		os << e.type << "," << colors::ToString(e.startColor) << "," << colors::ToString(e.endColor) << "," << (int)e.easeType;
		return os;
	}
	template<>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<Vector2>& e)
	{
		os << e.type << ",\"" << e.xVector << "\",\"" << e.yVector << "\"" << "," << (int)e.easeType;
		return os;
	}
}
