#pragma warning( disable : 4267 )
#pragma once
namespace sf
{
	class RenderWindow;
}

#include <SFML/Graphics/VertexArray.hpp>

#include "../../Types/Vector2i.h"

namespace maoutch
{
	class Sprite
	{
	public:
		Sprite(const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition);
		~Sprite();
		
		void SetFramePosition(const Vector2i& framePosition);
		void SetFrameSize(const Vector2i& frameSize);

		void Draw(sf::RenderWindow& window, const sf::Transform& transform);

	private:
		sf::RenderStates _renderState;
		sf::VertexArray _vertexArray;
		
		Vector2i _framePosition;
		Vector2i _frameSize;

		void _SetFrame();
	};
}
