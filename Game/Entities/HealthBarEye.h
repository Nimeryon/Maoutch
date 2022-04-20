#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/Sprite.h"

namespace maoutch
{
	class HealthBarEye : public GameObject
	{
	public:
		HealthBarEye();
		~HealthBarEye() override;

		void FixedUpdate(float dt) override;

	private:
		Sprite* _sprite;
		Vector2 _originalPosition;
		Vector2 _originalGlobalPosition;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
