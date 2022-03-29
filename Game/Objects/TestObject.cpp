#include <SFML/Graphics.hpp>

#include "TestObject.h"
#include "../../Tools/Transform.h"
#include "../../Engine/InputHandler.h"
#include "../../Engine/Objects/GameObjectHandler.h"

namespace maoutch
{
	TestObject::TestObject(const Vector2& position) : GameObject("Test Object"), _rectangle(new sf::RectangleShape())
	{
		SetPosition(position);
		SetOrigin(Vector2(8));

		_rectangle->setSize(Vector2(16));
		_rectangle->setFillColor(sf::Color(255, 0, 0, 96));
	}
	TestObject::~TestObject()
	{
		delete _rectangle;
	}

	void TestObject::ProcessInputs()
	{
		Vector2 mousePos = InputHandler::GetInstance()->GetMousePosition();
		if (transform::Contains(*_rectangle, _transform, mousePos))
			if (InputHandler::GetInstance()->IsPointerDown(sf::Mouse::Left))
				GameObjectHandler::instance->Destroy(this);
	}
	void TestObject::_OnDraw(sf::RenderWindow& window)
	{
		window.draw(*_rectangle, _transform.getTransform());
	}
}
