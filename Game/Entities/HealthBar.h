#pragma once
namespace sf
{
	class RectangleShape;
	class Transformable;
}

#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/Sprite.h"

namespace maoutch
{
	struct HealthBarPart
	{
		HealthBarPart(Sprite* sprite, const Vector2i& offset);

		Sprite* sprite;
		sf::Transformable transform;
	};

	class HealthBar : public GameObject
	{
	public:
		HealthBar(const float& health, const bool& isBossBar = false, int width = 3);
		~HealthBar() override;

		void FixedUpdate(float dt) override;

		void Damage(const float& damage);
		void SetIsBossBar(const bool& isBossBar);

	private:
		sf::RectangleShape* _healthRectangle;
		std::vector<HealthBarPart> _decorators;

		Vector2 _size;
		float _currentHealth;
		float _health;
		bool _isBossBar;
		float _scale;

		bool _breathIn;
		float _breathScale;
		float _currentBreathTime;
		float _breathTime;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
