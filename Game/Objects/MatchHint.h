#pragma once
#include "../../Engine/Graphics/AnimatedSprite.h"
#include "../../Engine/Objects/GameObject.h"
#include "../Match/MatchFinder.h"

namespace maoutch
{
	class MatchHint : public GameObject
	{
	public:
		MatchHint();
		~MatchHint() override;

		void SetActive(const bool& isActive) override;
		void SetPossibleMatch(const PossibleMatch& possibleMatch);

	private:
		static constexpr float _animationTime = .1f;

		PossibleMatch _possibleMatch;

		AnimatedSprite _hintAnimationNorth;
		AnimatedSprite _hintAnimationEast;
		AnimatedSprite _hintAnimationSouth;
		AnimatedSprite _hintAnimationWest;

		void _StopAnimations();
		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
