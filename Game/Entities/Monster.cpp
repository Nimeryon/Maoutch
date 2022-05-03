#include <fstream>
#include <json.hpp>

#include "Monster.h"
#include "HealthBar.h"
#include "../Match/Objects/MatchGridBackGround.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Assets.h"
#include "../Effects/Breather.h"
#include "../Effects/Floater.h"
#include "../Effects/Squisher.h"

namespace maoutch
{
	Monster::Monster(const std::string& fileName, const Element& element) :
		GameObject("Monster"),
		_element(element)
	{
		std::ifstream file(Assets::Config<std::string>("Monster", "Path") + fileName + ".json");
		nlohmann::json json = nlohmann::json::parse(file);

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

		// HealthBar
		HealthBar* healthBar = new HealthBar(
			json["BaseHealth"].get<int>(),
			json["Boss"].get<bool>()
		);
		healthBar->SetPosition(Vector2(0, -100));
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

	void Monster::Init()
	{
		// Setup Grid Backgrounds
		std::vector<MatchGridBackGround*> backgrounds = GameObjectHandler::Instance()->GetObjects<MatchGridBackGround>();
		for (auto* background : backgrounds)
			background->SetElement(_element);
	}

	void Monster::Damage(const float& damage)
	{
		GetChildren<HealthBar>("HealthBar")->Damage(damage);

		if (damage < 0)
		{
			Vector2 squishScale = Vector2(math::Clamp(-0.05f, -0.5f, damage * -0.1f));
			Squisher::Instance()->Apply({ _sprite, 0.1f, 0.05f, squishScale });
		}
	}

	Element Monster::GetElement() const { return _element; }
	Vector2 Monster::GetMonsterPosition() const
	{
		return GetGlobalPosition() + _sprite->GetPosition();
	}

	void Monster::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite->Draw(window, transform);
	}
}
