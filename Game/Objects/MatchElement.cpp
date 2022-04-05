#include <SFML/Graphics.hpp>

#include "MatchElement.h"
#include "../../Engine/InputHandler.h"
#include "../../Tools/Transform.h"
#include "../../Tools/Random.h"
#include "../../Engine/AssetLoader.h"
#include "../../Types/Direction.h"
#include "MatchGrid.h"
#include "../Match/Element.h"

extern maoutch::Vector2 ELEMENT_SIZE;
extern float ELEMENT_SNAP_DISTANCE;
extern float ELEMENT_DT_MULTIPLIER;
extern float ELEMENT_SWIPE_DISTANCE;

namespace maoutch
{
	MatchElement::MatchElement(MatchGrid& grid, const Vector2& startPos, const Vector2i& gridPos, const Element& element) :
		GameObject("MatchBox"),
		_type(element),
		_grid(grid),
		_gridPos(gridPos),
		_sprite(new sf::Sprite()),
		_isMatched(false),
		_isSelectd(false),
		_isMoving(false),
		_moveTimerFinished(false),
		_moveTimer(random::Float(0, 2), &MatchElement::_OnMoveToPositionTimer, this),
		_selectedAnimation(.05f, &AssetLoader::GetInstance()->GetTexture("Selected Animation"), Vector2i::From(ELEMENT_SIZE), Vector2i::Zero(), true)
	{
		_sprite->setTexture(AssetLoader::GetInstance()->GetTexture("Elements"));
		_sprite->setTextureRect(
			sf::IntRect(Vector2i((int)_type.Value(), 0) * ELEMENT_SIZE, ELEMENT_SIZE)
		);

		for (int i = 0; i < 12; ++i)
			_selectedAnimation.AddFrame({ i, 0 });
		
		SetPosition(startPos);
		SetOrigin(ELEMENT_SIZE / 2.f);
	}
	MatchElement::~MatchElement()
	{
		delete _sprite;
	}

	void MatchElement::ProcessInputs()
	{
		if (_grid.GetState() != GridState::INPUTS) return;

		Vector2 mousePos = InputHandler::GetInstance()->GetMousePosition();
		if (!_isSelectd && transform::Contains(*_sprite, _transform, mousePos))
			if (InputHandler::GetInstance()->IsPointerDown(sf::Mouse::Left)) OnPointerDown();

		if (_isSelectd)
		{
			if (InputHandler::GetInstance()->IsPointer(sf::Mouse::Left)) OnPointerUpdate();
			if (InputHandler::GetInstance()->IsPointerUp(sf::Mouse::Left)) OnPointerUp();
		}
	}
	void MatchElement::Update(float dt)
	{
		if (_moveTimerFinished && _isMoving)
		{
			// Move Horizontal
			if (std::abs(GetPosition().x - _goalPosition.x) > ELEMENT_SNAP_DISTANCE)
				SetPosition({ GetPosition().Lerp(_goalPosition, dt * ELEMENT_DT_MULTIPLIER).x, GetPosition().y});
			else SetPosition({ _goalPosition.x, GetPosition().y });

			// Move Vertical
			if (std::abs(GetPosition().y - _goalPosition.y) > ELEMENT_SNAP_DISTANCE)
				SetPosition({ GetPosition().x, GetPosition().Lerp(_goalPosition, dt * ELEMENT_DT_MULTIPLIER).y });
			else SetPosition({ GetPosition().x, _goalPosition.y });

			if (GetPosition() == _goalPosition)
			{
				_isMoving = false;
				_moveTimerFinished = false;
				SetZIndex(0);
			}
		}
	}
	void MatchElement::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		window.draw(*_sprite, transform);

		if (IsSelected())
			_selectedAnimation.Draw(window, transform);
	}

	Element MatchElement::GetElement() const { return _type; }
	Vector2i MatchElement::GetGridPos() const { return _gridPos; }
	bool MatchElement::IsMatched() const { return _isMatched; }
	bool MatchElement::IsSelected() const { return _isSelectd; }
	bool MatchElement::IsMoving() const { return _isMoving; }

	void MatchElement::SetElement(const Element& element)
	{
		_type = element;
		_sprite->setTextureRect(
			sf::IntRect(Vector2i((int)_type.Value(), 0) * ELEMENT_SIZE, ELEMENT_SIZE)
		);
	}
	void MatchElement::SetIsMatched()
	{
		_isMatched = true;
		_sprite->setColor(_isMatched ? sf::Color(255, 255, 255, 128) : sf::Color::White);
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
	void MatchElement::SetAndMoveToGridPos(const Vector2i& gridPos, const float& minStartMoveTime, const float& maxStartMoveTime)
	{
		SetGridPos(gridPos);
		MoveToGridPos(minStartMoveTime, maxStartMoveTime);
	}
	void MatchElement::SetAndMoveToGridPos(const Vector2i& gridPos, const float& moveTime)
	{
		SetAndMoveToGridPos(gridPos, moveTime, moveTime);
	}
	void MatchElement::MoveToGridPos(const float& minStartMoveTime, const float& maxStartMoveTime)
	{
		_isMoving = true;
		_goalPosition = _grid.GetCenterGridPosition(_gridPos);
		_moveTimer.SetTime(random::Float(minStartMoveTime, maxStartMoveTime));
		_moveTimer.Start();
	}
	void MatchElement::MoveToPos(const Vector2& position, const float& minStartMoveTime, const float& maxStartMoveTime)
	{
		_isMoving = true;
		_goalPosition = position;
		_moveTimer.SetTime(random::Float(minStartMoveTime, maxStartMoveTime));
		_moveTimer.Start();
	}
	void MatchElement::MoveToPos(const Vector2& position, const float& moveTime)
	{
		MoveToPos(position, moveTime, moveTime);
	}

	void MatchElement::UpdatePosition()
	{
		SetPosition(_grid.GetCenterGridPosition(_gridPos));
	}
	void MatchElement::OnPointerDown()
	{
		SetIsSelected(true);
		_startClickPosition = InputHandler::GetInstance()->GetMousePosition();
	}
	void MatchElement::OnPointerUpdate()
	{
		_endClickPosition = InputHandler::GetInstance()->GetMousePosition();
		if (_endClickPosition.Distance(_startClickPosition) > ELEMENT_SWIPE_DISTANCE)
		{
			// Calculate swipe angle
			// Angle between 0 and 360 deg
			float angle = std::atan2f(
				_endClickPosition.y - _startClickPosition.y,
				_endClickPosition.x - _startClickPosition.x
			) * RAD2DEG + 180;

			const Direction direction = Direction::GetDirection(angle, _transform.getRotation());
			_grid.Swap(_gridPos, direction);
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
		SetZIndex(1);
	}
}
