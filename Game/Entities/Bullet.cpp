#include "Bullet.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Engine/Assets.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Graphics/TextPopUp.h"

namespace maoutch
{
	Bullet::Bullet(const Vector2& position, const Element& element, const Vector2& particleSize) :
		GameObject("Bullet", 5),
		_particleScale(particleSize),
		_startPosition(position),
		_element(element),
		_monster(GameObjectHandler::Instance()->GetObject<Monster>("Monster")),
		_rotationSpeed(random::Float(
			-Assets::Config<float>("Bullet", "MaxRotationSpeed"),
			Assets::Config<float>("Bullet", "MaxRotationSpeed")
		)),
		_currentTravelTime(0),
		_travelTime(Assets::Config<float>("Bullet", "TravelTime"))
	{
		// Position
		SetPosition(position);

		// Generate CurvePoint
		// Generate the curve point on two rectangles at the left and right of the position
		// Can't generate a point on top or under the position to avoid some slow feeling bullets
		const Vector2 curveRect = Vector2::FromString(Assets::Config<std::string>("Bullet", "CurvePointRectangle"));
		const bool leftSide = random::Bool();
		const float pointOffset = Assets::Config<float>("Bullet", "CurvePointOffset");
		
		if (leftSide) _curvePoint = Vector2(
			random::Float(position.x - pointOffset - curveRect.x, position.x - pointOffset),
			random::Float(position.y - curveRect.y / 2.f, position.y + curveRect.y / 2.f)
		);
		else _curvePoint = Vector2(
			random::Float(position.x + pointOffset, position.x + pointOffset + curveRect.x),
			random::Float(position.y - curveRect.y / 2.f, position.y + curveRect.y / 2.f)
		);

		// Offset Range
		const float offsetRange = Assets::Config<float>("Bullet", "GoalOffsetRange");
		_goalOffset = Vector2(
			random::Float(-offsetRange, offsetRange),
			random::Float(-offsetRange, offsetRange)
		);

		// Offset Travel Time
		_travelTime -= random::Float(0, Assets::Config<float>("Bullet", "RandomTravelOffset"));

		// Particles
		_particleEmitter = SpawnParticle("Trails\\" + _element.ToString() + "_trail_particle.json");
	}

	void Bullet::Update(float dt)
	{
		Rotate(_rotationSpeed * dt);

		_currentTravelTime += dt;
		const float t = _currentTravelTime / _travelTime;
		static constexpr easing::EaseType easeType = easing::EaseType::EaseInQuad;

		const Vector2 monsterPosition = _monster->GetMonsterPosition() + _goalOffset;
		SetPosition(Vector2::Lerp(_startPosition.Lerp(_curvePoint, t, easeType), _curvePoint.Lerp(monsterPosition, t, easeType), t, easeType));
		_particleEmitter->SetPosition(GetPosition());

		if (_currentTravelTime >= _travelTime)
			GameObjectHandler::Instance()->Destroy(this);
	}
	void Bullet::OnDestroy()
	{
		// Calculate damage
		const Element monsterElement = _monster->GetElement();

		// Damage
		sf::Color popUpColor = sf::Color::Yellow;
		float damage = -1;
		std::string popUpText = string::ToString(damage);
		if (monsterElement.GetStrength() == _element)
		{
			popUpColor = sf::Color::Red;
			damage = -0.75;
			popUpText = string::ToString(damage);
		}
		else if (monsterElement.GetWeakness() == _element)
		{
			popUpColor = sf::Color::Green;
			damage = -1.75;
			popUpText = string::ToString(damage) + "!";
		}
		_monster->Damage(damage);

		// Damage PopUp
		new TextPopUp(popUpText, GetGlobalPosition(), 1, 14, popUpColor);

		// Particles
		_particleEmitter->SetDestroyAfterPlaying(true);
		_particleEmitter->Stop();
		SpawnParticle("Explosions\\" + _element.ToString() + "_explosion_particle.json");
	}

	ParticleEmitter* Bullet::SpawnParticle(const std::string& fileName) const
	{
		auto* emitter = new ParticleEmitter();
		emitter->SetZIndex(4);
		emitter->SetScale(_particleScale);
		emitter->SetPosition(GetPosition());
		emitter->SetupFromFile(fileName);

		return emitter;
	}
}
