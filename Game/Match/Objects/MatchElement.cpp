#include <SFML/Graphics.hpp>

#include "MatchElement.h"
#include "../../Engine/InputHandler.h"
#include "../../Engine/Assets.h"
#include "../../Tools/Transform.h"
#include "../../Tools/Random.h"
#include "../../Types/Direction.h"
#include "MatchGrid.h"
#include "../Data/Element.h"

namespace maoutch
{
	MatchElement::MatchElement(const Vector2& startPos, const Vector2i& gridPos, const Element& element) :
		GameObject("MatchBox", 0, false),
		_element(element),
		_gridPos(gridPos),
		_sprite(Sprite(
			Assets::Instance()->GetTexture("Elements"),
			Assets::Config<int>("Element", "Size"),
			Vector2i::Zero()
		)),
		_selectedAnimation(AnimatedSprite(
			Assets::Config<float>("Element", "SelectedAnimationTime"),
			Assets::Instance()->GetTexture("Selected Animation"), 
			Vector2i::From({ Assets::Config<float>("Element", "Size") }),
			Vector2i::Zero(), true
		)),
		_isMatched(false),
		_isSelectd(false),
		_isMoving(false),
		_moveTimerFinished(false),
		_disableAfterMoving(false),
		_moveTimer(random::Float(0, 2), &MatchElement::_OnMoveToPositionTimer, this),
		_currentMoveTime(0),
		_easeType(easing::EaseType::None)
	{
		for (int i = 0; i < 12; ++i)
			_selectedAnimation.AddFrame({ i, 0 });

		const Vector2 elementSize = { Assets::Config<float>("Element", "Size") };
		SetOrigin(elementSize / 2.f);
		SetPosition(startPos);

		SetElement(_element);
	}
	MatchElement::~MatchElement() = default;

	void MatchElement::ProcessInputs()
	{
		auto* grid = (MatchGrid*)parent;
		if (grid->GetState() != GridState::Inputs) return;

		Vector2 mousePos = InputHandler::Instance()->GetMousePosition();
		if (!_isSelectd && transform::Contains(_sprite, _transform, mousePos))
			if (InputHandler::Instance()->IsPointerDown(sf::Mouse::Left)) OnPointerDown();

		if (_isSelectd)
		{
			if (InputHandler::Instance()->IsPointer(sf::Mouse::Left)) OnPointerUpdate();
			if (InputHandler::Instance()->IsPointerUp(sf::Mouse::Left)) OnPointerUp();
		}
	}
	void MatchElement::Update(float dt)
	{
		if (_moveTimerFinished && _isMoving)
		{
			_currentMoveTime += dt;

			const float moveTime = Assets::Config<float>("Element", "MoveTime");
			float t = _currentMoveTime / moveTime;
			SetPosition(_startMovePosition.Lerp(_goalPosition, t, _easeType));

			if (_currentMoveTime >= moveTime)
			{
				_isMoving = false;
				_moveTimerFinished = false;
				SetZIndex(0);
				SetPosition(_goalPosition);

				if (_disableAfterMoving) SetActive(false);
			}
		}
	}
	void MatchElement::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite.Draw(window, transform);

		if (IsSelected()) _selectedAnimation.Draw(window, transform);
	}

	Element MatchElement::GetElement() const { return _element; }
	Vector2i MatchElement::GetGridPos() const { return _gridPos; }
	bool MatchElement::IsMatched() const { return _isMatched; }
	bool MatchElement::IsSelected() const { return _isSelectd; }
	bool MatchElement::IsMoving() const { return _isMoving; }

	void MatchElement::SetElement(const Element& element)
	{
		_element = element;
		_sprite.SetFramePosition(Vector2i((int)_element.Value(), 0));
	}
	void MatchElement::SetIsMatched()
	{
		_isMatched = true;
		_sprite.SetColor(_isMatched ? sf::Color(255, 255, 255, 128) : sf::Color::White);
	}
	void MatchElement::SetIsSelected(const bool& isSelected)
	{
		_isSelectd = isSelected;

		if (_isSelectd) _selectedAnimation.Play();
		else _selectedAnimation.Stop();
	}

	void MatchElement::SetGridPos(const Vector2i& gridPos)
	{
		_gridPos = gridPos;
		SetName((std::string)gridPos + " Element");
	}
	void MatchElement::SetAndMoveToGridPos(const Vector2i& gridPos, const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		SetGridPos(gridPos);
		MoveToGridPos(minStartMoveTime, maxStartMoveTime, easeType, disableAfterMove);
	}
	void MatchElement::SetAndMoveToGridPos(const Vector2i& gridPos, const float& moveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		SetAndMoveToGridPos(gridPos, moveTime, moveTime, easeType, disableAfterMove);
	}
	void MatchElement::MoveToGridPos(const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		auto* grid = (MatchGrid*)parent;

		_easeType = easeType;
		_isMoving = true;
		_disableAfterMoving = disableAfterMove;
		_goalPosition = grid->GetCenterGridPosition(_gridPos);
		_moveTimer.SetTime(random::Float(minStartMoveTime, maxStartMoveTime));
		_moveTimer.Start();
	}
	void MatchElement::MoveToGridPos(const float& moveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		MoveToGridPos(moveTime, moveTime, easeType, disableAfterMove);
	}
	void MatchElement::MoveToPos(const Vector2& position, const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		_easeType = easeType;
		_isMoving = true;
		_disableAfterMoving = disableAfterMove;
		_goalPosition = position;
		_moveTimer.SetTime(random::Float(minStartMoveTime, maxStartMoveTime));
		_moveTimer.Start();
	}
	void MatchElement::MoveToPos(const Vector2& position, const float& moveTime, const easing::EaseType& easeType, const bool& disableAfterMove)
	{
		MoveToPos(position, moveTime, moveTime, easeType, disableAfterMove);
	}

	void MatchElement::UpdatePosition()
	{
		auto* grid = (MatchGrid*)parent;
		SetPosition(grid->GetCenterGridPosition(_gridPos));
	}
	void MatchElement::OnPointerDown()
	{
		SetIsSelected(true);
		_startClickPosition = InputHandler::Instance()->GetMousePosition();
	}
	void MatchElement::OnPointerUpdate()
	{
		_endClickPosition = InputHandler::Instance()->GetMousePosition();
		if (_endClickPosition.Distance(_startClickPosition) > Assets::Config<float>("Element", "SwipeDistance"))
		{
			// Calculate swipe angle
			// Angle between 0 and 360 deg
			float angle = std::atan2f(
				_endClickPosition.y - _startClickPosition.y,
				_endClickPosition.x - _startClickPosition.x
			) * RAD2DEG + 180;

			const Direction direction = Direction::GetDirection(angle, GetGlobalRotation());
			auto* grid = (MatchGrid*)parent;
			grid->Swap(_gridPos, direction);
			OnPointerUp();
		}
	}
	void MatchElement::OnPointerUp()
	{
		SetIsSelected(false);
	}

	void MatchElement::_OnMoveToPositionTimer()
	{
		_moveTimerFinished = true;
		_currentMoveTime = 0;
		_startMovePosition = GetPosition();
		SetVisible(true);
		SetZIndex(1);
	}
}
