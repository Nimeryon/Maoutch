#include "Shaker.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	Shaker* Shaker::_instance = nullptr;
	std::mutex Shaker::_mutex;

	ShakeData::ShakeData(ITransformable* object, const float& time, const float& maxRadius, const float& magnitude, const float& decreaseFactor) :
		EffectData(object, time),
		initialPosition(object->GetPosition()),
		maxRadius(maxRadius),
		magnitude(magnitude),
		decreaseFactor(decreaseFactor)
	{}

	void ShakeData::operator=(const ShakeData& data)
	{
		currentTime = 0;
		maxRadius = data.maxRadius;
		magnitude = data.magnitude;
		decreaseFactor = data.decreaseFactor;
		time = data.time;
	}

	Shaker::Shaker() = default;
	Shaker::~Shaker() = default;

	Shaker* Shaker::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Shaker();
		return _instance;
	}

	bool Shaker::_UpdateEffect(const float& dt, ShakeData& data, ITransformable* transformable)
	{
		if (data.currentTime < data.time)
		{
			data.currentTime += dt;

			Vector2 randomDirection = Vector2::RandomCireclePoint();
			data.magnitude -= data.decreaseFactor * dt;
			if (data.magnitude < 0) data.magnitude = 0;

			transformable->Move(randomDirection * dt * data.magnitude);
			if (transformable->GetPosition().Distance(data.initialPosition) > data.maxRadius)
				transformable->SetPosition(data.initialPosition);
		}
		else
		{
			transformable->SetPosition(data.initialPosition);
			return false;
		}

		return true;
	}
}
