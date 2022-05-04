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
		Sprite();
		Sprite(const sf::Texture& texture, const Vector2i& frameSize, const Vector2i& framePosition);
		virtual ~Sprite();

		// Setters

		void SetColor(const sf::Color& color);
		void SetFramePosition(const Vector2i& framePosition);
		void SetFrameSize(const Vector2i& frameSize);

		// Getters

		sf::Color GetColor() const;

		void Draw(sf::RenderWindow& window, const sf::Transform& transform);

		sf::FloatRect GetBounds() const override;

	protected:
		sf::RenderStates _renderState;
		sf::VertexArray _vertexArray;

		Vector2i _frameSize;
		Vector2i _framePosition;
		sf::Color _color;

	private:
		virtual void _SetFrame();
	};
}
