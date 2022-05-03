#pragma once
#include <SFML/Graphics.hpp>
#include "../../Types/Vector2.h"

namespace maoutch
{
	class ITransformable
	{
	public:
		virtual sf::FloatRect GetBounds() const;

		[[nodiscard]] virtual Vector2 GetPosition() const;
		[[nodiscard]] virtual Vector2 GetScale() const;
		[[nodiscard]] virtual Vector2 GetOrigin() const;
		[[nodiscard]] virtual float GetRotation() const;

		virtual void SetPosition(const Vector2& position);
		virtual void SetRotation(float angle);
		virtual void SetScale(const Vector2& factor);
		virtual void SetOrigin(const Vector2& origin);
		virtual void Move(const Vector2& offset);
		virtual void Rotate(float angle);
		virtual void Scale(const Vector2& factor);

	protected:
		sf::Transformable _localTransform;
	};
}
