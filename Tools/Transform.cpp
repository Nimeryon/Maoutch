#include <SFML/Graphics.hpp>

#include "Transform.h"
#include "../Interfaces/ITransformable.h"

namespace maoutch
{
	namespace transform
	{
		sf::Transformable Compose(const sf::Transformable& target, const sf::Transformable& from)
		{
			sf::Transform fromTransfrom = from.getTransform();

			sf::Transformable transformable = target;
			transformable.setPosition(fromTransfrom.transformPoint(transformable.getPosition()));
			transformable.rotate(from.getRotation());
			transformable.scale(from.getScale());

			return transformable;
		}

		sf::FloatRect GetRect(sf::FloatRect rect, const sf::Transformable& transform)
		{
			rect.left = transform.getPosition().x - (transform.getOrigin().x * transform.getScale().x);
			rect.top = transform.getPosition().y - (transform.getOrigin().y * transform.getScale().y);
			rect.width = rect.width * transform.getScale().x;
			rect.height = rect.height * transform.getScale().y;

			return rect;
		}
		
		bool Contains(const ITransformable& transformable, const sf::Transformable& transform, const Vector2& point)
		{
			return GetRect(transformable.GetBounds(), transform).contains(point);
		}

		bool Intersects(const ITransformable& transformable1, const sf::Transformable& transform1, const ITransformable& transformable2, const sf::Transformable& transform2)
		{
			return GetRect(transformable1.GetBounds(), transform1).intersects(GetRect(transformable2.GetBounds(), transform2));
		}


		std::array<sf::Vertex, 4> GetVertices(const sf::FloatRect& rect)
		{
			std::array<sf::Vertex, 4> vertices;

			// Position
			vertices[0].position = { rect.left, rect.top };
			vertices[1].position = { rect.left + rect.width, rect.top };
			vertices[2].position = { rect.left + rect.width, rect.top + rect.height };
			vertices[3].position = { rect.left, rect.top + rect.height };

			return vertices;
		}
	}
}
