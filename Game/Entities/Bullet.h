#pragma once
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Graphics/ParticleEmitter.h"
#include "../Match/Data/Element.h"
#include "Monster.h"

namespace maoutch
{
	class Bullet : public GameObject
	{
	public:
		Bullet(const Vector2& position, const Element& element, const Vector2& particleScale = Vector2::One());

		void Update(float dt) override;

		void OnDestroy() override;

	private:
		ParticleEmitter* _particleEmitter;
		Monster* _monster;

		Vector2 _particleScale;
		Vector2 _goalOffset;
		Vector2 _startPosition;
		Vector2 _curvePoint;
		Element _element;

		float _rotationSpeed;
		float _currentTravelTime;
		float _travelTime;

		ParticleEmitter* SpawnParticle(const std::string& fileName) const;
	};
}
