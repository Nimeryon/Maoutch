#include <SFML/Graphics/Vertex.hpp>
#include <array>

#include "Texture.h"

namespace maoutch
{
	namespace texture
	{
		void SetTextureCoord(std::array<sf::Vertex, 4>& vertices, const Vector2i& position, const Vector2i& size)
		{
			// Offset texture position by a rectangle of 0.0001f to avoid texture overlapping
			Vector2 texturePosition = position * size;
			vertices[0].texCoords = texturePosition + Vector2(0.0001f, 0.0001f);
			vertices[1].texCoords = Vector2(texturePosition.x + size.x, texturePosition.y) + Vector2(-0.0001f, 0.0001f);
			vertices[2].texCoords = texturePosition + size + Vector2(-0.0001f, -0.0001f);
			vertices[3].texCoords = Vector2(texturePosition.x, texturePosition.y + size.y) + Vector2(0.0001f, -0.0001f);
		}
	}
}
