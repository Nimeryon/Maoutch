#pragma once
namespace sf
{
	class RectangleShape;
}
#include "../../Engine/Objects/GameObject.h"
#include "../../Types/Vector2.h"

namespace maoutch
{
	class TestObject : public GameObject
	{
	public:
		TestObject(const Vector2& position);
		~TestObject() override;

		void ProcessInputs() override;
		void _OnDraw(sf::RenderWindow& window) override;

	private:
		sf::RectangleShape* _rectangle;
	};
}
