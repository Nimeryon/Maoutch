#pragma warning( disable : 4244 )
#pragma once
namespace sf { class Color; }
#include <string>

namespace maoutch
{
	namespace colors
	{
		// Lerp between two colors. t clamped between 0 and 1
		sf::Color LerpRGB(sf::Color a, sf::Color b, float t);

		sf::Color FromFloat4(float color[4]);
		void ToFloat4(float* values, sf::Color color);

		std::string ToString(sf::Color color);
		sf::Color FromString(std::string color);
	}
}