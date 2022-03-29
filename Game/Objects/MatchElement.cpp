#include <SFML/Graphics.hpp>

#include "MatchElement.h"
#include "../../Engine/InputHandler.h"
#include "../../Tools/Transform.h"
#include "../../Tools/Random.h"
#include "../../Engine/AssetLoader.h"
#include "../../Types/Direction.h"
#include "MatchGrid.h"
#include "../Match/Element.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern maoutch::Vector2 ELEMENT_SIZE;
extern maoutch::Vector2 ELEMENT_SCALE;
extern float ELEMENT_SNAP_DISTANCE;
extern float ELEMENT_DT_MULTIPLIER;
extern float ELEMENT_SWIPE_DISTANCE;

namespace maoutch
{
	MatchElement::MatchElement(MatchGrid& grid, const Vector2i& gridPos, const Element& element) :
		GameObject("MatchBox"),
		_type(element),
		_grid(grid),
		_gridPos(gridPos),
		_sprite(new sf::Sprite()),
		_isMatched(false),
		_isSelectd(false),
		_isMoving(false)
	{
		_sprite->setTexture(AssetLoader::GetInstance()->GetTexture("Elements"));
		_sprite->setTextureRect(
			sf::IntRect(Vector2i((int)_type.Value(), 0) * ELEMENT_SIZE, ELEMENT_SIZE)
		);

		Vector2 goalPos = _grid.GetCenterGridPosition(_gridPos);
		SetPosition({ goalPos.x, -WINDOW_HEIGHT / 2.f + random::Float(-256, -34) });
		SetScale(ELEMENT_SCALE);
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
		if (_isMoving)
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
				SetZIndex(0);
			}
		}
	}
	void MatchElement::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		window.draw(*_sprite, transform);
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
	void MatchElement::SetGridPos(const Vector2i& gridPos)
	{
		_gridPos = gridPos;
		SetName((std::string)gridPos + " Element");
	}
	void MatchElement::SetAndMoveToGridPos(const Vector2i& gridPos)
	{
		SetGridPos(gridPos);
		MoveToGridPos();
	}
	void MatchElement::SetIsMatched()
	{
		_isMatched = true;
		_sprite->setColor(_isMatched ? sf::Color(64, 64, 64, 255) : sf::Color::White);
	}
	void MatchElement::SetIsSelected(const bool& isSelected) { _isSelectd = isSelected; }

	void MatchElement::MoveToGridPos()
	{
		Vector2 goalPos = _grid.GetCenterGridPosition(_gridPos);
		if (GetPosition() != goalPos)
		{
			_isMoving = true;
			_goalPosition = goalPos;
			SetZIndex(1);
		}
	}
	void MatchElement::UpdatePosition()
	{
		SetPosition(_grid.GetCenterGridPosition(_gridPos));
	}
	void MatchElement::OnPointerDown()
	{
		_isSelectd = true;
		_startClickPosition = InputHandler::GetInstance()->GetMousePosition();
	}
	void MatchElement::OnPointerUpdate()
	{
		_endClickPosition = InputHandler::GetInstance()->GetMousePosition();
		if (_endClickPosition.Distance(_startClickPosition) > ELEMENT_SWIPE_DISTANCE)
		{
			float angle = std::atan2f(
				_endClickPosition.y - _startClickPosition.y,
				_endClickPosition.x - _startClickPosition.x
			) * RAD2DEG;
			const Direction direction = Direction::GetDirection(angle);
			_grid.Swap(_gridPos, direction);
			OnPointerUp();
		}
	}
	void MatchElement::OnPointerUp()
	{
		_isSelectd = false;
	}
}
