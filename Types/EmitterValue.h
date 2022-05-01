#pragma once
#include <SFML/Graphics/Color.hpp>

#include "LerpableValue.h"
#include "json.hpp"
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

		nlohmann::json ToJson()
		{
			nlohmann::json json;

			json["Type"] = type;
			json["MinXValue"] = minXValue;
			json["MaxXValue"] = minYValue;
			json["MinYValue"] = maxXValue;
			json["MaxYValue"] = maxYValue;
			json["EaseType"] = (int)easeType;

			return json;
		}
		void FromJson(const nlohmann::json& json)
		{
			type = json.at("Type").get<int>();
			minXValue = json.at("MinXValue").get<T>();
			minYValue = json.at("MaxXValue").get<T>();
			maxXValue = json.at("MinYValue").get<T>();
			maxYValue = json.at("MaxYValue").get<T>();
			easeType = easing::EaseType(json.at("EaseType").get<int>());
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

		nlohmann::json ToJson()
		{
			nlohmann::json json;

			json["Type"] = type;
			json["StartColor"] = colors::ToString(startColor);
			json["EndColor"] = colors::ToString(endColor);
			json["EaseType"] = (int)easeType;

			return json;
		}
		void FromJson(const nlohmann::json& json)
		{
			type = json.at("Type").get<int>();
			startColor = colors::FromString(json.at("StartColor").get<std::string>());
			endColor = colors::FromString(json.at("EndColor").get<std::string>());
			easeType = easing::EaseType(json.at("EaseType").get<int>());
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
					return LerpableValue(xVector, easeType);

				case 1:
					return LerpableValue(Vector2(random::Float(xVector.x, xVector.y), random::Float(yVector.x, yVector.y)).Normalized(), easeType);

				default:
					return LerpableValue(xVector, yVector, easeType);
			}
		}

		nlohmann::json ToJson()
		{
			nlohmann::json json; 

			json["Type"] = type;
			json["XVector"] = (std::string)xVector;
			json["YVector"] = (std::string)yVector;
			json["EaseType"] = (int)easeType;

			return json;
		}
		void FromJson(const nlohmann::json& json)
		{
			type = json.at("Type").get<int>();
			xVector = Vector2::FromString(json.at("XVector").get<std::string>());
			yVector = Vector2::FromString(json.at("YVector").get<std::string>());
			easeType = easing::EaseType(json.at("EaseType").get<int>());
		}

		int type;
		Vector2 xVector;
		Vector2 yVector;
		easing::EaseType easeType;
	};
}
