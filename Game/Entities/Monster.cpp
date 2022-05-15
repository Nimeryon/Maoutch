#include <fstream>
#include <json.hpp>

#include "Monster.h"
#include "HealthBar.h"
#include "../Match/Objects/MatchGridBackGround.h"
#include "../Match/Objects/MatchGrid.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Assets.h"
#include "../../Tools/Random.h"
#include "../Effects/Breather.h"
#include "../Effects/Floater.h"
#include "../Effects/Squisher.h"

namespace maoutch
{
	int Monster::level = 0;

	Monster::Monster(const std::string& fileName, const Element& element) :
		GameObject("Monster", -2),
		_element(element)
	{
		level++;

		std::ifstream file(Assets::Config<std::string>("Monster", "Path") + fileName + ".json");
		nlohmann::json json = nlohmann::json::parse(file);

		// Position
		const float windowWidth = Assets::Config<float>("Window", "Width");
		SetPosition(Vector2(windowWidth / 2.f, 150));

		// Boss
		_isBoss = level % 10 == 0;

		// Health
		const float baseHealth = json["BaseHealth"].get<float>();
		const float bossHealthMult = _isBoss ? 2 : 1;

		_health = 0;
		for (int i = 1; i <= level; ++i)
			_health += baseHealth + (i - 1) * 2 * bossHealthMult;

		// Sprite
		const Vector2i size = Vector2i::FromString(json["Size"].get<std::string>());
		float screenSize = 200.f;
		if (_isBoss) screenSize = json["BossSize"].get<float>();
		_scale = screenSize / size.x;

		_sprite = new Sprite(
			Assets::Instance()->GetTexture(json["Texture"].get<std::string>()),
			size,
			Vector2i((int)_element.Value(), 0)
		);
		_sprite->SetOrigin((Vector2)size / 2.f);
		_sprite->SetScale(_scale);

		// Animations
		if (json["Breathing"].get<bool>())
		{
			Breather::Instance()->Apply({
				_sprite,
				json["BreathTime"].get<float>(),
				Vector2::FromString(json["BreathScale"].get<std::string>()) * _scale,
				easing::EaseType::EaseInSine,
				easing::EaseType::EaseOutSine
				});
		}

		if (json["Floating"].get<bool>())
		{
			Floater::Instance()->Apply({
				_sprite,
				json["FloatingTime"].get<float>(),
				json["FloatingHeight"].get<float>() * _scale,
				easing::EaseType::EaseInOutSine,
				easing::EaseType::EaseInOutSine
			});
		}
	}
	Monster::~Monster()
	{
		HealthBar* healthBar = GameObjectHandler::Instance()->GetObjects<HealthBar>()[0];
		healthBar->onDeath -= EventHandler::Bind(&Monster::_OnDeath, this);

		GameObjectHandler::Instance()->NotifyDestroy(_sprite);
	}

	void Monster::CreateRandomMonster()
	{
		std::vector<std::string> monsters = {
			"cerbere",
			"demon"
		};
		new Monster(monsters[random::Int(0, monsters.size())], Element::Random());
	}

	void Monster::Init()
	{
		// Setup Grid Backgrounds
		std::vector<MatchGridBackGround*> backgrounds = GameObjectHandler::Instance()->GetObjects<MatchGridBackGround>();
		for (auto* background : backgrounds)
			background->SetElement(_element);
		
		// HealthBar Handler
		HealthBar* healthBar = GameObjectHandler::Instance()->GetObjects<HealthBar>()[0];
		healthBar->SetIsBossBar(_isBoss);
		healthBar->SetHealth(_health);
		healthBar->onDeath += EventHandler::Bind(&Monster::_OnDeath, this);
	}

	void Monster::Damage(const float& damage)
	{
		GameObjectHandler::Instance()->GetObjects<HealthBar>()[0]->Damage(damage);

		if (damage < 0)
		{
			Vector2 squishScale = Vector2(math::Clamp(-0.05f, -0.5f, damage * -0.1f));
			Squisher::Instance()->Apply({ _sprite, 0.1f, 0.05f, squishScale * _scale });
		}
	}

	Element Monster::GetElement() const { return _element; }
	Vector2 Monster::GetMonsterPosition() const
	{
		return GetGlobalPosition() + _sprite->GetPosition();
	}

	void Monster::_OnDeath()
	{
		GameObjectHandler::Instance()->Destroy(this);

		// std::string grids[3] = { "big_grid", "circle_grid", "small_grid" };
		// GameObjectHandler::Instance()->GetObjects<MatchGrid>()[0]->Setup(grids[random::Int(0, 2)] );

		CreateRandomMonster();
	}

	void Monster::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_sprite->Draw(window, transform);
	}
}
