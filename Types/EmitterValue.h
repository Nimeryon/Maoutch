#pragma once
#include <SFML/Graphics/Color.hpp>

#include "LerpableValue.h"
#include "Vector2.h"
#include "../../Tools/Random.h"
#include "../../Tools/Colors.h"
#include "../../Tools/String.h"

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
		EmitterValue(const T& minXValue, const T& maxXValue, const T& minYValue, const T& maxYValue, const int& type) :
			type(type),
			minXValue(minXValue),
			maxXValue(maxXValue),
			minYValue(minYValue),
			maxYValue(maxYValue) {}
		EmitterValue(const T& minValue, const T& maxValue, const int& type) :
			type(type),
			minXValue(minValue),
			maxXValue(maxValue),
			minYValue(minValue),
			maxYValue(maxValue) {}
		EmitterValue(T value) :
			type(0),
			minXValue(value),
			maxXValue(value),
			minYValue(value),
			maxYValue(value) {}

		[[nodiscard]] LerpableValue<T> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<T>(minXValue);

				case 1:
					return LerpableValue<T>(random::Float(minXValue, maxXValue));

				case 2:
					return LerpableValue<T>(minXValue, maxXValue);

				default:
					return LerpableValue<T>(random::Float(minXValue, maxXValue), random::Float(minYValue, maxYValue));
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
		}

		int type;
		T minXValue;
		T minYValue;
		T maxXValue;
		T maxYValue;
	};

	template<>
	struct EmitterValue<sf::Color>
	{
		EmitterValue(const sf::Color& startColor, const sf::Color& endColor, const int& type) :
			type(type),
			startColor(startColor),
			endColor(endColor) {}
		EmitterValue(const sf::Color& color) :
			type(0),
			startColor(color),
			endColor(color) {}
		EmitterValue() : type(1) {}

		[[nodiscard]] LerpableValue<sf::Color> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<sf::Color>(startColor);

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

					return LerpableValue<sf::Color>(colors::FromFloat4(color1), colors::FromFloat4(color2));
				}

				default:
					return LerpableValue<sf::Color>(startColor, endColor);
			}
		}
		void FromString(std::string value)
		{
			const auto values = string::Split(value, ',');
			type = std::atoi(&values[0][0]);
			startColor = colors::FromString(values[1]);
			endColor = colors::FromString(values[2]);
		}

		int type;
		sf::Color startColor;
		sf::Color endColor;
	};

	template<>
	struct EmitterValue<Vector2>
	{
		EmitterValue(const Vector2& xVector, const Vector2& yVector, const int& type) :
			type(type),
			xVector(xVector),
			yVector(yVector) {}
		EmitterValue(const Vector2& vector) :
			type(0),
			xVector(vector),
			yVector(vector) {}
		EmitterValue() : type(1) {}

		[[nodiscard]] LerpableValue<Vector2> GetValue() const
		{
			switch (type)
			{
				case 0:
					return LerpableValue<Vector2>(xVector);

				case 1:
					return LerpableValue<Vector2>(Vector2(random::Float(xVector.x, xVector.y), random::Float(yVector.x, yVector.y)).Normalized());

				default:
					return LerpableValue<Vector2>(xVector, yVector);
			}
		}
		void FromString(std::string value)
		{
			const auto values = string::Split(value, ',');
			type = std::atoi(&values[0][0]);
			xVector = Vector2::FromString(string::Replace(values[1], "\"", ""));
			yVector = Vector2::FromString(string::Replace(values[2], "\"", ""));
		}

		int type;
		Vector2 xVector;
		Vector2 yVector;
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<T>& e)
	{
		os << e.type << "," << e.minXValue << "," << e.maxXValue << "," << e.minYValue << "," << e.maxYValue;
		return os;
	}
	template<>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<sf::Color>& e)
	{
		os << e.type << "," << colors::ToString(e.startColor) << "," << colors::ToString(e.endColor);
		return os;
	}
	template<>
	inline std::ostream& operator<<(std::ostream& os, const EmitterValue<Vector2>& e)
	{
		os << e.type << ",\"" << e.xVector << "\",\"" << e.yVector << "\"";
		return os;
	}
}
