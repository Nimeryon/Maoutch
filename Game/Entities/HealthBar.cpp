#include <SFML/Graphics.hpp>

#include "HealthBar.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "Monster.h"
#include "../../Types/Vector2i.h"
#include "../../Tools/Colors.h"
#include "../../Tools/Math.h"
#include "../../Engine/Assets.h"
#include "../Effects/Shaker.h"
#include "HealthBarSkull.h"

namespace maoutch
{
	HealthBar::HealthBar(const float& health, const bool& isBossBar) :
		GameObject("HealthBar", 100),
		_healthText(new Text(string::ToString(health))),
		_levelText(new Text("Lvl: " + string::ToString(Monster::level))),
		_currentHealth(health),
		_health(health),
		_isBossBar(isBossBar),
		_indicatorHealth(health),
		_currentIndicatorTime(0),
		_indicatorTime(Assets::Config<float>("HealthBar", "IndicatorTime")),
		_startedIndicating(false),
		_isIndicating(false),
		_indicatorTimer(Assets::Config<float>("HealthBar", "IndicatorDelay"), &HealthBar::_OnIndicatorTimer, this)
	{
		Vector2 partSize = Vector2(
			Assets::Config<float>("HealthBar", "PartWidth"),
			Assets::Config<float>("HealthBar", "PartHeight")
		);
		_size = Vector2(partSize.x * 6, partSize.y);
		_rectangleSize = _size - 6;

		SetScale(Assets::Config<float>("HealthBar", "Scale"));
		SetOrigin(_size / 2.f);
		
		for (int x = 0; x < 6; ++x)
		{
			int side = 1;
			if (x == 0) side = 0;
			else if (x == 6 - 1) side = 2;

			_decorators.emplace_back(new Sprite(
				Assets::Instance()->GetTexture("HealthBar"),
				Vector2i::From(partSize),
				Vector2i(side, 0)
			));
			_decorators[x]->Move(Vector2i(partSize.x * x, 0));
		}

		// Health rectangles
		const Vector2 origin = Vector2(-3, -3 - _rectangleSize.y / 4.f);
		_SetupSize();

		_backgroundRectangle.setOrigin(origin);
		_damageIndicatorRectangle.setOrigin(origin);
		_healthRectangle.setOrigin(origin);

		// Health skull
		HealthBarSkull* skull = new HealthBarSkull();
		skull->SetPosition(Vector2::FromString(Assets::Config<std::string>("HealthBar", "SkullPosition")));
		AddChildren(skull);

		// Health Text
		_healthText->SetColor(sf::Color::White);
		_healthText->SetSize(16);
		_healthText->SetOrigin(Vector2(_healthText->GetBounds().width, 0) / 2.f);

		// Lvel Text
		_levelText->SetColor(sf::Color::White);
		_levelText->SetSize(16);
		_levelText->SetOrigin(Vector2(_levelText->GetBounds().width, 0));

		// Boss
		SetIsBossBar(_isBossBar);
	}
	HealthBar::~HealthBar()
	{
		for (Sprite* sprite : _decorators)
			GameObjectHandler::Instance()->NotifyDestroy(sprite);
		GameObjectHandler::Instance()->NotifyDestroy(_healthText);
	}

	void HealthBar::FixedUpdate(float dt)
	{
		// Move HealthText and LevelText to position
		_healthText->SetPosition(GetGlobalPosition());
		_levelText->SetPosition(GetGlobalPosition() + Vector2(_size.x - 8, 0));

		if (_isIndicating)
		{
			_currentIndicatorTime += dt;
			if (_currentIndicatorTime >= _indicatorTime)
			{
				_currentIndicatorTime = _indicatorTime;
				_isIndicating = false;
				_startedIndicating = false;

				// Events
				if (_currentHealth == 0) onDeath();
			}

			const float timerT = _currentIndicatorTime / _indicatorTime;
			_indicatorHealth = math::Lerp(_indicatorHealth, _currentHealth, Ease(easing::EaseType::EaseInExpo, timerT));
			const float t = _indicatorHealth / _health;
			_damageIndicatorRectangle.setSize(Vector2(math::Clamp(_size.x * t - 6, 0.f, _size.x), _damageIndicatorRectangle.getSize().y));
		}
	}

	void HealthBar::Damage(const float& damage)
	{
		_startedIndicating = true;
		_indicatorTimer.Restart();

		// Update health
		_currentHealth = math::Clamp(_currentHealth += damage, 0.f, _health);

		const float t = _currentHealth / _health;
		_healthRectangle.setSize(Vector2(math::Clamp(_size.x * t - 6, 0.f, _size.x), _healthRectangle.getSize().y));
		_healthRectangle.setFillColor(colors::LerpRGB(sf::Color::Red, sf::Color::Green, t, easing::EaseType::EaseInOutQuad));

		_healthText->SetText(string::ToString(_currentHealth));
		_healthText->SetOrigin(Vector2(_healthText->GetBounds().width, 0) / 2.f);

		// Events
		if (damage > 0) onHeal(damage);
		else
		{
			Shaker::Instance()->Apply({
				this,
				0.25,
				2,
				150,
				150
			});
			onDamage(damage);
		}
	}
	void HealthBar::SetIsBossBar(const bool& isBossBar)
	{
		_isBossBar = isBossBar;
		GetChildren<HealthBarSkull>("HealthBar Skull")->SetActive(_isBossBar);

		//decorators[0]->SetFramePosition(Vector2i(_isBossBar ? 3 : 0, 0));
	}
	void HealthBar::SetHealth(const float& health)
	{
		_currentHealth = health;
		_health = health;

		_healthText->SetText(string::ToString(_currentHealth));
		_healthText->SetOrigin(Vector2(_healthText->GetBounds().width, 0) / 2.f);

		_levelText->SetText("Lvl: " + string::ToString(Monster::level));
		_levelText->SetOrigin(Vector2(_levelText->GetBounds().width, 0));

		_SetupSize();
	}

	void HealthBar::_OnIndicatorTimer()
	{
		// Setup Indicator
		_isIndicating = true;
		_currentIndicatorTime = 0;
	}
	void HealthBar::_SetupSize()
	{
		_backgroundRectangle.setSize(Vector2(_rectangleSize.x, _rectangleSize.y / 2.f));
		_backgroundRectangle.setFillColor(sf::Color(66, 66, 66));

		_damageIndicatorRectangle.setSize(Vector2(_rectangleSize.x, _rectangleSize.y / 2.f));
		_damageIndicatorRectangle.setFillColor(sf::Color::Red);

		_healthRectangle.setSize(Vector2(_rectangleSize.x, _rectangleSize.y / 2.f));
		_healthRectangle.setFillColor(sf::Color::Green);
	}

	void HealthBar::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		window.draw(_backgroundRectangle, transform);
		if (_startedIndicating) window.draw(_damageIndicatorRectangle, transform);
		window.draw(_healthRectangle, transform);
		
		for (auto& decorator : _decorators)
			decorator->Draw(window, transform);

		_healthText->Draw(window, transform);
		_levelText->Draw(window, transform);
	}
}
