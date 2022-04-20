#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/AnimatedSprite.h"
#include "../../Engine/Tools/Timer.h"

namespace maoutch
{
	class HealthBarSkull : public GameObject
	{
	public:
		HealthBarSkull();
		~HealthBarSkull() override;

		void SetActive(const bool& isActive) override;

	private:
		AnimatedSprite* _animatedSprite;

		Timer<HealthBarSkull> _blinkTimer;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;

		void _OnBlink();
	};
}
