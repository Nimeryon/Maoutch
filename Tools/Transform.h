#pragma once
namespace sf
{
	class Shape;
	class Sprite;
	class Transformable;
}


#include "../Types/Vector2.h"

namespace maoutch
{
	namespace transform
	{
		sf::Transformable Compose(const sf::Transformable& target, const sf::Transformable& from);

		sf::FloatRect GetRect(sf::FloatRect rect, const sf::Transformable& transform);

		bool Contains(const sf::Shape& shape, const sf::Transformable& transform, const Vector2& point);
		bool Contains(const sf::Sprite& sprite, const sf::Transformable& transform, const Vector2& point);

		bool Intersects(const sf::Shape& shape1, const sf::Transformable& transform1, const sf::Shape& shape2, const sf::Transformable& transform2);
		bool Intersects(const sf::Sprite& sprite1, const sf::Transformable& transform1, const sf::Sprite& sprite2, const sf::Transformable& transform2);
	}
}
