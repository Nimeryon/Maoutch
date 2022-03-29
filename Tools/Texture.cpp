#include <SFML/Graphics.hpp>
#include <array>

#include "Texture.h"

namespace maoutch
{
	namespace texture
	{
		void SetTextureCoord(std::array<sf::Vertex, 4>& vertices, const Vector2i& position, const Vector2i& size)
		{
			Vector2i texturePosition = position * size;
			vertices[0].texCoords = texturePosition;
			vertices[1].texCoords = { (float)texturePosition.x + size.x, (float)texturePosition.y };
			vertices[2].texCoords = texturePosition + size;
			vertices[3].texCoords = { (float)texturePosition.x, (float)texturePosition.y + size.y };
		}
	}
}
