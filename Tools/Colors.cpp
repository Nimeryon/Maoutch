#include <SFML/Graphics.hpp>

#include "Colors.h"
#include "Math.h"
#include "String.h"

namespace maoutch
{
	namespace colors
	{
		sf::Color LerpRGB(sf::Color a, sf::Color b, float t, const easing::EaseType& easeType)
		{
			t = math::Clamp(t, 0.f, 1.f);
			return sf::Color(
				a.r + (b.r - a.r) * Ease(easeType, t),
				a.g + (b.g - a.g) * Ease(easeType, t),
				a.b + (b.b - a.b) * Ease(easeType, t),
				a.a + (b.a - a.a) * Ease(easeType, t)
			);
		}

		sf::Color FromFloat4(float color[4])
		{
			return sf::Color((int)(color[0] * 255), (int)(color[1] * 255), (int)(color[2] * 255), (int)(color[3] * 255));
		}
		void ToFloat4(float* values, sf::Color color)
		{
			values[0] = color.r / 255.f;
			values[1] = color.g / 255.f;
			values[2] = color.b / 255.f;
			values[3] = color.a / 255.f;
		}

		std::string ToString(sf::Color color)
		{
			std::string res = "\"";
			res += std::to_string(color.r) + "|";
			res += std::to_string(color.g) + "|";
			res += std::to_string(color.b) + "|";
			res += std::to_string(color.a) + "\"";

			return res;
		}
		sf::Color FromString(std::string color)
		{
			color = string::Replace(color, "\"", "");
			std::vector<std::string> values = string::Split(color, '|');
			return sf::Color(std::atof(&values[0][0]), std::atof(&values[1][0]), std::atof(&values[2][0]), std::atof(&values[3][0]));
		}
	}
}