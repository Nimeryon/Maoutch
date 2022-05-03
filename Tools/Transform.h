#pragma once
namespace sf
{
	class Vertex;
	class Transformable;
}
#include <array>

#include "../Types/Vector2.h"

namespace maoutch
{
	class ITransformable;

	namespace transform
	{
		sf::Transformable Compose(const sf::Transformable& target, const sf::Transformable& from);

		sf::FloatRect GetRect(sf::FloatRect rect, const sf::Transformable& transform);

		bool Contains(const ITransformable& transformable, const sf::Transformable& transform, const Vector2& point);

		bool Intersects(const ITransformable& transformable1, const sf::Transformable& transform1, const ITransformable& transformable2, const sf::Transformable& transform2);

		std::array<sf::Vertex, 4> GetVertices(const sf::FloatRect& rect);
	}
}
