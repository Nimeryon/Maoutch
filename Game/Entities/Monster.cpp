#include <fstream>
#include <json.hpp>

#include "Monster.h"
#include "HealthBar.h"
#include "../../Engine/Assets.h"
#include "../Effects/Breather.h"
#include "../Effects/Floater.h"

namespace maoutch
{
	Monster::Monster(const std::string& fileName) : GameObject("Monster")
	{
		std::ifstream file(Assets::Config<std::string>("Monster", "Path") + fileName + ".json");
		nlohmann::json json = nlohmann::json::parse(file);

		// Element
		_element = Element::FromString(json["Element"].get<std::string>());

		// Sprite
		const Vector2i size = Vector2i(json["Size"].get<float>());
		const float scale = json["Scale"].get<float>();
		_sprite = new Sprite(
			&Assets::Instance()->GetTexture(json["Texture"].get<std::string>()),
			size,
			Vector2i((int)_element.Value(), 0)
		);
		_sprite->SetOrigin((Vector2)size / 2.f);
		_sprite->SetScale(scale);

		// Shadow
		const Vector2i shadowSize = Vector2i(Assets::Config<int>("Monster", "ShadowSize"));
		_shadowSprite = new Sprite(
			&Assets::Instance()->GetTexture("Shadow"),
			shadowSize,
			Vector2i::Zero()
		);
		_shadowSprite->SetOrigin((Vector2)shadowSize / 2.f);
		_shadowSprite->SetScale(Vector2::FromString(json["ShadowScale"].get<std::string>()));
		_shadowSprite->SetPosition(Vector2(0, json["ShadowHeight"].get<float>()));

		// HealthBar
		HealthBar* healthBar = new HealthBar(
			json["BaseHealth"].get<int>(),
			json["Boss"].get<bool>(),
			json["HealthWidth"].get<int>()
		);
		healthBar->SetPosition(Vector2(0, json["HealthHeight"].get<float>()));
		AddChildren(healthBar);

		// Animations
		if (json["Breathing"].get<bool>())
		{
			Breather::Instance()->Apply({
				_sprite,
				json["BreathTime"].get<float>(),
				Vector2::FromString(json["BreathScale"].get<std::string>()),
				easing::EaseType::EaseInSine,
				easing::EaseType::EaseOutSine
				});
			Breather::Instance()->Apply({
				_shadowSprite,
				json["BreathTime"].get<float>(),
				Vector2::FromString(json["BreathScale"].get<std::string>()),
				easing::EaseType::EaseInSine,
				easing::EaseType::EaseOutSine
				});
		}

		if (json["Floating"].get<bool>())
		{
			Floater::Instance()->Apply({
				_sprite,
				json["FloatingTime"].get<float>(),
				json["FloatingHeight"].get<float>(),
				easing::EaseType::EaseInOutSine,
				easing::EaseType::EaseInOutSine
			});
		}
	}
	Monster::~Monster()
	{
		delete _sprite;
	}

	void Monster::Damage(const float& damage)
	{
		GetChildren<HealthBar>("HealthBar")->Damage(damage);
	}

	void Monster::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_shadowSprite->Draw(window, transform);
		_sprite->Draw(window, transform);
	}
}
