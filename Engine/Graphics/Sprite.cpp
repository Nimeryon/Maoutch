#include <array>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Tools/Texture.h"
#include "../../Tools/Transform.h"
#include "Sprite.h"

namespace maoutch
{
	Sprite::Sprite(const sf::Texture& texture, const Vector2i& frameSize, const Vector2i& framePosition) :
		_frameSize(frameSize),
		_framePosition(framePosition),
		_color(sf::Color::White)
	{
		_renderState.texture = &texture;
		_vertexArray.setPrimitiveType(sf::Quads);

		_SetFrame();
	}
	Sprite::~Sprite() = default;

	void Sprite::SetColor(const sf::Color& color)
	{
		_color = color;
		_SetFrame();
	}
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

	sf::Color Sprite::GetColor() const { return _color; }

	void Sprite::Draw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_renderState.transform = transform * _localTransform.getTransform();
		window.draw(_vertexArray, _renderState);
	}
	
	sf::FloatRect Sprite::GetBounds() const	{ return _vertexArray.getBounds(); }

	void Sprite::_SetFrame()
	{
		_vertexArray.clear();

		std::array<sf::Vertex, 4> vertices = transform::GetVertices(sf::FloatRect(Vector2::Zero(), _frameSize));
		texture::SetTextureCoord(vertices, _framePosition, _frameSize);

		for (sf::Vertex& vertex : vertices)
		{
			vertex.color = _color;
			_vertexArray.append(vertex);
		}
	}
}
