#include <array>
#include "Sprite.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Tools/Texture.h"

namespace maoutch
{
	Sprite::Sprite(const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition) :
		_framePosition(framePosition),
		_frameSize(frameSize)
	{
		_renderState.texture = texture;
		_vertexArray.setPrimitiveType(sf::Quads);

		_SetFrame();
	}
	Sprite::~Sprite() = default;

	void Sprite::SetFramePosition(const Vector2i& framePosition)
	{
		_framePosition = framePosition;
		_SetFrame();
	}
	void Sprite::SetFrameSize(const Vector2i& frameSize)
	{
		_frameSize = frameSize;
		_SetFrame();
	}

	void Sprite::Draw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_renderState.transform = transform * _localTransform.getTransform();
		window.draw(_vertexArray, _renderState);
	}

	void Sprite::_SetFrame()
	{
		_vertexArray.clear();

		std::array<sf::Vertex, 4> vertices;
		texture::SetTextureCoord(vertices, _framePosition, _frameSize);

		vertices[0].position = { 0, 0 };
		vertices[1].position = { (float)_frameSize.x, 0 };
		vertices[2].position = _frameSize;
		vertices[3].position = { 0, (float)_frameSize.y };

		for (sf::Vertex& vertex : vertices)
			_vertexArray.append(vertex);
	}
}
