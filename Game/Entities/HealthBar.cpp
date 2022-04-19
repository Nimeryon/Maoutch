#include <SFML/Graphics.hpp>

#include "HealthBar.h"
#include "../../Types/Vector2i.h"
#include "../../Tools/Colors.h"
#include "../../Tools/Math.h"
#include "../../Engine/Assets.h"
#include "HealthBarSkull.h"

namespace maoutch
{
	HealthBarPart::HealthBarPart(Sprite* sprite, const Vector2i& offset) :
		sprite(sprite)
	{
		transform.move(offset);
	}

	HealthBar::HealthBar(const float& health, const bool& isBossBar, int width) :
		GameObject("HealthBar", 100),
		_healthRectangle(new sf::RectangleShape()),
		_currentHealth(health),
		_health(health),
		_isBossBar(isBossBar)
	{
		Vector2 partSize = Vector2(
			Assets::Config<float>("HealthBar", "PartSize"),
			Assets::Config<float>("HealthBar", "PartSize") / 2.f
		);
		_size = Vector2(partSize.x * width, partSize.y);

		SetOrigin(_size / 2.f);

		if (width < 3) width = 3;
		for (int x = 0; x < width; ++x)
		{
			int side = 1;
			if (x == 0) side = 0;
			else if (x == width - 1) side = 2;

			_decorators.emplace_back(HealthBarPart(
				new Sprite(
					&Assets::GetInstance()->GetTexture("HealthBar"),
					Vector2i::From(partSize),
					Vector2i(side, 0)
				),
				Vector2i(partSize.x * x, 0)
			));
		}

		_healthRectangle->setSize(Vector2(_size.x - 6, _size.y - 6));
		_healthRectangle->setOrigin(-3, -3);
		_healthRectangle->setFillColor(sf::Color::Green);

		HealthBarSkull* skull = new HealthBarSkull();
		skull->SetOrigin(Vector2(Assets::Config<float>("HealthBar", "SkullSize") / 2.f, Assets::Config<float>("HealthBar", "SkullOriginY")));
		skull->SetPosition(Vector2(_size.x / 2.f, 0));
		AddChildren(skull);

		SetIsBossBar(_isBossBar);
	}
	HealthBar::~HealthBar()
	{
		delete _healthRectangle;
		for (auto& decorator : _decorators)
			delete decorator.sprite;
	}
	
	void HealthBar::Damage(const float& damage)
	{
		if (_currentHealth - damage <= 0.f)
		{
			_currentHealth = _health;
		}

		_currentHealth -= damage;

		const float t = _currentHealth / _health;
		_healthRectangle->setSize(Vector2(math::Clamp(_size.x * t - 6, 0.f, _size.x), _size.y - 6));
		_healthRectangle->setFillColor(colors::LerpRGB(sf::Color::Red, sf::Color::Green, t, easing::EaseType::EaseInOutQuad));
	}
	void HealthBar::SetIsBossBar(const bool& isBossBar)
	{
		_isBossBar = isBossBar;
		GetChildren("HealthBar Skull")->SetActive(_isBossBar);

		if (_isBossBar) SetScale(Vector2(2.f));
		else SetScale(1.5f);
	}

	void HealthBar::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		window.draw(*_healthRectangle, transform);
		
		for (auto& decorator : _decorators)
			decorator.sprite->Draw(window, transform * decorator.transform.getTransform());
	}
}
