#include <SFML/Graphics.hpp>
#include "Transform.h"

namespace maoutch
{
	namespace transform
	{
		sf::Transformable Compose(const sf::Transformable& target, const sf::Transformable& from)
		{
			sf::Transformable transform = target;
			transform.move(from.getPosition());
			transform.rotate(from.getRotation());
			transform.scale(from.getScale());

			return transform;
		}

		sf::FloatRect GetRect(sf::FloatRect rect, const sf::Transformable& transform)
		{
			rect.left = transform.getPosition().x - (transform.getOrigin().x * transform.getScale().x);
			rect.top = transform.getPosition().y - (transform.getOrigin().y * transform.getScale().y);
			rect.width = rect.width * transform.getScale().x;
			rect.height = rect.height * transform.getScale().y;

			return rect;
		}

		bool Contains(const sf::Shape& shape, const sf::Transformable& transform, const Vector2& point)
		{
			return GetRect(shape.getGlobalBounds(), transform).contains(point);
		}
		bool Contains(const sf::Sprite& sprite, const sf::Transformable& transform, const Vector2& point)
		{
			return GetRect(sprite.getGlobalBounds(), transform).contains(point);
		}

		bool Intersects(const sf::Shape& shape1, const sf::Transformable& transform1, const sf::Shape& shape2, const sf::Transformable& transform2)
		{
			return GetRect(shape1.getGlobalBounds(), transform1).intersects(GetRect(shape2.getGlobalBounds(), transform2));
		}
		bool Intersects(const sf::Sprite& sprite1, const sf::Transformable& transform1, const sf::Sprite& sprite2, const sf::Transformable& transform2)
		{
			return GetRect(sprite1.getGlobalBounds(), transform1).intersects(GetRect(sprite2.getGlobalBounds(), transform2));
		}
	}
}
