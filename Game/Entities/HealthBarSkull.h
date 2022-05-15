#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/AnimatedSprite.h"
#include "../../Engine/Graphics/Sprite.h"
#include "../../Engine/Tools/Timer.h"

namespace maoutch
{
	class HealthBarSkull : public GameObject
	{
	public:
		HealthBarSkull();
		~HealthBarSkull() override;

		void ProcessInputs() override;
		void FixedUpdate(float dt) override;

	private:
		AnimatedSprite* _animatedSprite;
		Sprite* _decorator;
		Sprite* _leftEyeSprite;
		Sprite* _rightEyeSprite;

		Timer<HealthBarSkull> _blinkTimer;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;

		void _OnBlink();
	};
}
