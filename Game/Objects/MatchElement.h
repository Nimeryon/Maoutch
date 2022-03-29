#pragma once
namespace sf
{
	class Sprite;
}

#include "../../Engine/Objects/GameObject.h"
#include "../../Types//Vector2i.h"
#include "../Match/Element.h"

namespace maoutch
{
	class MatchGrid;

	class MatchElement : public GameObject
	{
	public:
		MatchElement(MatchGrid& grid, const Vector2i& gridPos, const Element& element);
		~MatchElement() override;

		void ProcessInputs() override;
		void Update(float dt) override;
		void _OnDraw(sf::RenderWindow& window) override;

		Vector2i GetGridPos() const;
		Element GetElement() const;
		bool IsMatched() const;
		bool IsSelected() const;
		bool IsMoving() const;

		void SetElement(const Element& element);
		void SetGridPos(const Vector2i& gridPos);
		void SetAndMoveToGridPos(const Vector2i& gridPos);
		void SetIsMatched();
		void SetIsSelected(const bool& isSelected);

		void MoveToGridPos();
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
		Vector2 _startClickPosition;
		Vector2 _endClickPosition;
		Vector2 _goalPosition;
	};
}
