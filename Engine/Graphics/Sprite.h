#pragma warning( disable : 4267 )
#pragma once
namespace sf
{
	class RenderWindow;
}
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "../../Types/Vector2i.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	class Sprite : public ITransformable
	{
	public:
		Sprite(const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition);
		virtual ~Sprite();
		
		void SetFramePosition(const Vector2i& framePosition);
		void SetFrameSize(const Vector2i& frameSize);

		void Draw(sf::RenderWindow& window, const sf::Transform& transform);

	protected:
		sf::RenderStates _renderState;
		sf::VertexArray _vertexArray;

		Vector2i _frameSize;

	private:
		Vector2i _framePosition;

		virtual void _SetFrame();
	};
}
