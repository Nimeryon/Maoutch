#pragma once
namespace sf
{
	class Vertex;
}
#include "../Types/Vector2i.h"

namespace maoutch
{
	namespace texture
	{
		void SetTextureCoord(std::array<sf::Vertex, 4>& vertices, const Vector2i& position, const Vector2i& size);
	}
}
