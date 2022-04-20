#include "HealthBarSkull.h"
#include "HealthBarEye.h"
#include "../../Engine/Assets.h"
#include "../../Tools/Random.h"

namespace maoutch
{
	HealthBarSkull::HealthBarSkull() :
		GameObject("HealthBar Skull", 102),
		_blinkTimer(
			1,
			&HealthBarSkull::_OnBlink, 
			this,
			true
		)
	{
		const float objectProba = random::Float();
		std::vector<float> probabilities = Assets::Config().at("SkullProbabilities").get<std::vector<float>>();
		for (int i = 0; i < probabilities.size(); ++i)
			if (objectProba <= probabilities[i])
			{
				_objectDecorator = i;
				break;
			}

		_animatedSprite = new AnimatedSprite(
			Assets::Config<float>("HealthBar", "SkullAnimationTime"),
			&Assets::GetInstance()->GetTexture("HealthBar Skull"),
			Vector2i(Assets::Config<int>("HealthBar", "SkullSize")),
			Vector2i(0, _objectDecorator),
			false,
			true
		);
		_animatedSprite->AddFrame(Vector2i(1, _objectDecorator));
		_animatedSprite->AddFrame(Vector2i(2, _objectDecorator));
		_animatedSprite->AddFrame(Vector2i(2, _objectDecorator));
		_animatedSprite->AddFrame(Vector2i(1, _objectDecorator));

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
		_blinkTimer.Start();
	}
	HealthBarSkull::~HealthBarSkull()
	{
		delete _animatedSprite;
	}

	void HealthBarSkull::SetActive(const bool& isActive)
	{
		GetChildren("HealthBar Eye Left")->SetActive(isActive);
		GetChildren("HealthBar Eye Right")->SetActive(isActive);

		GameObject::SetActive(isActive);
	}

	void HealthBarSkull::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_animatedSprite->Draw(window, transform);
	}

	void HealthBarSkull::_OnBlink()
	{
		_blinkTimer.SetTime(random::Float(
			Assets::Config<float>("HealthBar", "SkullBlinkMinTime"),
			Assets::Config<float>("HealthBar", "SkullBlinkMaxTime")
		));

		_animatedSprite->Play();
	}

}
