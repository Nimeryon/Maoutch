#pragma once
#include "../../Engine/Objects/GameObject.h"

namespace maoutch
{
	class MatchHint : public GameObject
	{
	public:
		MatchHint();
		~MatchHint() override;

	private:
		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}