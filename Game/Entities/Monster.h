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
		Monster(const std::string& fileName, const Element& element);
		~Monster();

		void Init() override;

		void Damage(const float& damage) override;

		Element GetElement() const;
		Vector2 GetMonsterPosition() const;

	private:
		Element _element;
		Sprite* _sprite;

		float _scale;
		bool _isBoss;

		void _OnDeath();
		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
