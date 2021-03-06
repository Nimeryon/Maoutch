#pragma once
namespace sf
{
	class RectangleShape;
	class Transformable;
}

#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/Sprite.h"
#include "../../Engine/Graphics/Text.h"
#include "../../Interfaces/IDamageable.h"
#include "../../Tools/Event.h"
#include "../../Engine/Tools/Timer.h"

namespace maoutch
{
	class HealthBar : public GameObject, public IDamageable
	{
	public:
		HealthBar(const float& health = 10, const bool& isBossBar = false);
		~HealthBar() override;

		void FixedUpdate(float dt) override;

		void Damage(const float& damage) override;
		void SetIsBossBar(const bool& isBossBar);
		void SetHealth(const float& health);

		Event<void> onDeath;
		Event<const float> onDamage;
		Event<const float> onHeal;

	private:
		sf::RectangleShape _backgroundRectangle;
		sf::RectangleShape _damageIndicatorRectangle;
		sf::RectangleShape _healthRectangle;
		std::vector<Sprite*> _decorators;

		Text* _healthText;
		Text* _levelText;

		Vector2 _size;
		Vector2 _rectangleSize;

		float _currentHealth;
		float _health;
		bool _isBossBar;

		float _indicatorHealth;
		float _currentIndicatorTime;
		float _indicatorTime;
		bool _startedIndicating;
		bool _isIndicating;

		Timer<HealthBar> _indicatorTimer;

		void _OnIndicatorTimer();
		void _SetupSize();

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
