#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/Sprite.h"

namespace maoutch
{
	class HealthBarSkull : public GameObject
	{
	public:
		HealthBarSkull();
		~HealthBarSkull() override;

		void SetActive(const bool& isActive) override;

	private:
		Sprite* _sprite;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
