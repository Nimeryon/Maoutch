#include "HealthBarSkull.h"
#include "../../Engine/Assets.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Tools/Random.h"
#include "../../Tools/Transform.h"
#include "../../Engine/InputHandler.h"
#include "../Effects/Squisher.h"

namespace maoutch
{
	HealthBarSkull::HealthBarSkull() :
		GameObject("HealthBar Skull", 102),
		_animatedSprite(new AnimatedSprite(
			Assets::Config<float>("HealthBar", "SkullAnimationTime"),
			Assets::Instance()->GetTexture("HealthBar Skull"),
			Vector2i(Assets::Config<int>("HealthBar", "SkullSize")),
			Vector2i(0, 0),
			false,
			true
		)),
		_decorator(new Sprite(
			Assets::Instance()->GetTexture("Skull Decorators"),
			Vector2i(Assets::Config<int>("HealthBar", "DecoratorSize")),
			Vector2i::Zero()
		)),
		_leftEyeSprite(new Sprite(
			Assets::Instance()->GetTexture("HealthBar Eye"),
			Vector2i(Assets::Config<int>("HealthBar", "EyeSize")),
			Vector2i::Zero()
		)),
		_rightEyeSprite(new Sprite(
			Assets::Instance()->GetTexture("HealthBar Eye"),
			Vector2i(Assets::Config<int>("HealthBar", "EyeSize")),
			Vector2i::Zero()
		)),
		_blinkTimer(
			1,
			&HealthBarSkull::_OnBlink,
			this,
			true
		)
	{
		SetOrigin(Vector2(Assets::Config<float>("HealthBar", "SkullSize") / 2.f));

		const float diff = (Assets::Config<float>("HealthBar", "DecoratorSize") - Assets::Config<float>("HealthBar", "SkullSize")) / 2.f;
		_decorator->SetOrigin(diff);
		const float objectProba = random::Float();
		std::vector<float> probabilities = Assets::Config().at("SkullProbabilities").get<std::vector<float>>();
		for (int i = 0; i < probabilities.size(); ++i)
			if (objectProba <= probabilities[i])
			{
				_decorator->SetFramePosition(Vector2i(i, 0));
				break;
			}

		_animatedSprite->AddFrame(Vector2i(1, 0));
		_animatedSprite->AddFrame(Vector2i(2, 0));
		_animatedSprite->AddFrame(Vector2i(2, 0));
		_animatedSprite->AddFrame(Vector2i(1, 0));

		const Vector2 eyeOrigin = Vector2(Assets::Config<float>("HealthBar", "EyeSize") / 2.f);
		_leftEyeSprite->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "LeftEyePosX"),
			Assets::Config<float>("HealthBar", "LeftEyePosY")
		));
		_leftEyeSprite->SetOrigin(eyeOrigin);

		_rightEyeSprite->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "RightEyePosX"),
			Assets::Config<float>("HealthBar", "RightEyePosY")
		));
		_rightEyeSprite->SetOrigin(eyeOrigin);

		SetScale(Vector2(Assets::Config<float>("HealthBar", "SkullScale")));
		_blinkTimer.Start();
	}
	HealthBarSkull::~HealthBarSkull()
	{
		GameObjectHandler::Instance()->NotifyDestroy(_animatedSprite);
		GameObjectHandler::Instance()->NotifyDestroy(_decorator);
		GameObjectHandler::Instance()->NotifyDestroy(_leftEyeSprite);
		GameObjectHandler::Instance()->NotifyDestroy(_rightEyeSprite);
	}

	void HealthBarSkull::ProcessInputs()
	{
		Vector2 mousePos = InputHandler::Instance()->GetMousePosition();
		if (transform::Contains(*_animatedSprite, _transform, mousePos))
			if (InputHandler::Instance()->IsPointerDown(sf::Mouse::Left))
				Squisher::Instance()->Apply({ this, 0.1f, 0.05f, -0.1f });
	}
	void HealthBarSkull::FixedUpdate(float dt)
	{
		_leftEyeSprite->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "LeftEyePosX"),
			Assets::Config<float>("HealthBar", "LeftEyePosY")
		));
		_rightEyeSprite->SetPosition(Vector2(
			Assets::Config<float>("HealthBar", "RightEyePosX"),
			Assets::Config<float>("HealthBar", "RightEyePosY")
		));

		float rotation = -GetGlobalRotation() * DEG2RAD;
		Vector2 mousePos = InputHandler::Instance()->GetMousePosition();
		const Vector2 offset = Vector2(
			Assets::Config<float>("HealthBar", "EyeOffsetX"),
			Assets::Config<float>("HealthBar", "EyeOffsetY")
		);

		// Left Eye
		const Vector2 leftEyeGlobalPosition = Vector2::From(_transform.getTransform().transformPoint(_leftEyeSprite->GetPosition()));
		const Vector2 mouseDirectionLeft = (mousePos - leftEyeGlobalPosition).Normalized();
		const Vector2 finalMouseDirectionLeft = Vector2(
			mouseDirectionLeft.x * std::cosf(rotation) - mouseDirectionLeft.y * std::sinf(rotation),
			mouseDirectionLeft.x * std::sinf(rotation) + mouseDirectionLeft.y * std::cosf(rotation)
		);

		const float distanceLeft = leftEyeGlobalPosition.Distance(mousePos);
		const float distanceMultLeft = distanceLeft < 15 ? distanceLeft / 15 : 1;

		_leftEyeSprite->Move(Vector2(finalMouseDirectionLeft * offset * distanceMultLeft));

		// Right Eye
		const Vector2 rightEyeGlobalPosition = Vector2::From(_transform.getTransform().transformPoint(_rightEyeSprite->GetPosition()));
		const Vector2 mouseDirectionRight = (mousePos - rightEyeGlobalPosition).Normalized();
		const Vector2 finalMouseDirectionRight = Vector2(
			mouseDirectionRight.x * std::cosf(rotation) - mouseDirectionRight.y * std::sinf(rotation),
			mouseDirectionRight.x * std::sinf(rotation) + mouseDirectionRight.y * std::cosf(rotation)
		);

		const float distanceRight = rightEyeGlobalPosition.Distance(mousePos);
		const float distanceMultRight = distanceRight < 15 ? distanceRight / 15 : 1;

		_rightEyeSprite->Move(Vector2(finalMouseDirectionRight * offset * distanceMultRight));
	}

	void HealthBarSkull::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_leftEyeSprite->Draw(window, transform);
		_rightEyeSprite->Draw(window, transform);
		_animatedSprite->Draw(window, transform);
		_decorator->Draw(window, transform);
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
