#include "HealthBarEye.h"
#include "../../Engine/Assets.h"
#include "../../Engine/InputHandler.h"

namespace maoutch
{
	HealthBarEye::HealthBarEye() :
		GameObject("HealthBar Eye", 101),
		_sprite(new Sprite(
			&Assets::GetInstance()->GetTexture("HealthBar Eye"),
			Vector2i(Assets::Config<int>("HealthBar", "EyeSize")),
			Vector2i::Zero()
		))
	{}
	HealthBarEye::~HealthBarEye()
	{
		delete _sprite;
	}

	void HealthBarEye::FixedUpdate(float dt)
	{
		if (_originalPosition == Vector2::Zero())
			_originalPosition = GetPosition();
		SetPosition(_originalPosition);
		
		Vector2 mouseDirection = InputHandler::GetInstance()->GetMousePosition() - GetGlobalPosition();
		mouseDirection = mouseDirection.Normalized();

		Move(Vector2(
			Assets::Config<float>("HealthBar", "EyeOffsetX") * mouseDirection.x,
			Assets::Config<float>("HealthBar", "EyeOffsetY") * mouseDirection.y
		));
	}

	void HealthBarEye::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite->Draw(window, transform);
	}
}
