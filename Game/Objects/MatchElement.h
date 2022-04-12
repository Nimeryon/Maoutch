#pragma once
namespace sf
{
	class Sprite;
}

#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Tools/Timer.h"
#include "../../Tools/Easing.h"
#include "../../Types/Vector2i.h"
#include "../Match/Element.h"
#include "../../Engine/Graphics/AnimatedSprite.h"

namespace maoutch
{
	class MatchGrid;

	class MatchElement : public GameObject
	{
	public:
		MatchElement(MatchGrid& grid, const Vector2& startPos, const Vector2i& gridPos, const Element& element);
		~MatchElement() override;
		
		void ProcessInputs() override;
		void Update(float dt) override;

		Vector2i GetGridPos() const;
		Element GetElement() const;
		bool IsMatched() const;
		bool IsSelected() const;
		bool IsMoving() const;

		void SetElement(const Element& element);
		void SetIsMatched();
		void SetIsSelected(const bool& isSelected);

		void SetGridPos(const Vector2i& gridPos);
		void SetAndMoveToGridPos(const Vector2i& gridPos, const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);
		void SetAndMoveToGridPos(const Vector2i& gridPos, const float& moveTime = 0, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);
		void MoveToGridPos(const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);
		void MoveToGridPos(const float& moveTime = 0, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);
		void MoveToPos(const Vector2& position, const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);
		void MoveToPos(const Vector2& position, const float& moveTime = 0, const easing::EaseType& easeType = easing::EaseType::None, const bool& disableAfterMove = false);

		void UpdatePosition();
		void OnPointerDown();
		void OnPointerUpdate();
		void OnPointerUp();

	private:
		Element _type;

		MatchGrid& _grid;
		Vector2i _gridPos;
		sf::Sprite* _sprite;

		bool _isMatched;
		bool _isSelectd;
		bool _isMoving;
		bool _moveTimerFinished;
		bool _disableAfterMoving;
		Timer<MatchElement> _moveTimer;
		Vector2 _startClickPosition;
		Vector2 _endClickPosition;
		Vector2 _startMovePosition;
		Vector2 _goalPosition;
		float _currentMoveTime;
		easing::EaseType _easeType;

		AnimatedSprite _selectedAnimation;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;

		void _OnMoveToPositionTimer();
	};
}
