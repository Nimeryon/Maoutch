#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/Sprite.h"
#include "../Match/Data/Element.h"
#include "../../Interfaces/IDamageable.h"

namespace maoutch
{
	class Monster : public GameObject, public IDamageable
	{
	public:
		Monster(const std::string& fileName);
		~Monster();

		void Damage(const float& damage) override;

	private:
		Element _element;
		Sprite* _sprite;
		Sprite* _shadowSprite;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
