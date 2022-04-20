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
		{
			_originalPosition = GetPosition();
			_originalGlobalPosition = GetGlobalPosition();
		}
		SetPosition(_originalPosition);

		float rotation = -GetGlobalRotation() * DEG2RAD;
		Vector2 mousePos = InputHandler::GetInstance()->GetMousePosition();
		Vector2 mouseDirection = (mousePos - _originalGlobalPosition).Normalized();
		Vector2 finalMouseDirection = Vector2(
			mouseDirection.x * std::cosf(rotation) - mouseDirection.y * std::sinf(rotation),
			mouseDirection.x * std::sinf(rotation) + mouseDirection.y * std::cosf(rotation)
		);

		const float distance = _originalGlobalPosition.Distance(mousePos);
		const float distanceMult = distance < 15
			? distance / 15
			: 1;

		Move(Vector2(
			Assets::Config<float>("HealthBar", "EyeOffsetX") * (finalMouseDirection.x * distanceMult),
			Assets::Config<float>("HealthBar", "EyeOffsetY") * (finalMouseDirection.y * distanceMult)
		));
	}

	void HealthBarEye::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite->Draw(window, transform);
	}
}
