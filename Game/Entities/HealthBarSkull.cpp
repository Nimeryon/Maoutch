#include "HealthBarSkull.h"
#include "../../Engine/Assets.h"
#include "HealthBarEye.h"

namespace maoutch
{
	HealthBarSkull::HealthBarSkull() :
		GameObject("HealthBar Skull", 102),
		_sprite(new Sprite(
			&Assets::GetInstance()->GetTexture("HealthBar Skull"),
			Vector2i(Assets::Config<int>("HealthBar", "SkullSize")),
			Vector2i::Zero()
		))
	{
		HealthBarEye* leftEye = new HealthBarEye();
		leftEye->SetName("HealthBar Eye Left"); 
		leftEye->SetOrigin(Vector2(Assets::Config<float>("HealthBar", "EyeSize") / 2.f));
		leftEye->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "LeftEyePosX"),
			Assets::Config<float>("HealthBar", "LeftEyePosY")
		));
		AddChildren(leftEye);
		
		HealthBarEye* rightEye = new HealthBarEye();
		rightEye->SetName("HealthBar Eye Right");
		rightEye->SetOrigin(Vector2(Assets::Config<float>("HealthBar", "EyeSize") / 2.f));
		rightEye->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "RightEyePosX"),
			Assets::Config<float>("HealthBar", "RightEyePosY")
		));
		AddChildren(rightEye);

		SetScale(Vector2(Assets::Config<float>("HealthBar", "SkullScale")));
	}
	HealthBarSkull::~HealthBarSkull()
	{
		delete _sprite;
	}

	void HealthBarSkull::SetActive(const bool& isActive)
	{
		GetChildren("HealthBar Eye Left")->SetActive(isActive);
		GetChildren("HealthBar Eye Right")->SetActive(isActive);

		GameObject::SetActive(isActive);
	}

	void HealthBarSkull::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite->Draw(window, transform);
	}
}
