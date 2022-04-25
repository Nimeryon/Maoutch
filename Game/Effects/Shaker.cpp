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
	
	Shaker* Shaker::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Shaker();
		return _instance;
	}

	Shaker::Shaker() = default;
	Shaker::~Shaker() = default;

	bool Shaker::_UpdateEffects(const float& dt)
	{
		bool shaked = false;

		for (int i = 0; i < _effectDatas.size(); ++i)
		{
			ShakeData& data = _effectDatas[i];
			if (!data.object)
			{
				_effectDatas.erase(_effectDatas.begin() + i);
				continue;
			}

			if (data.currentTime < data.time)
			{
				shaked = true;
				data.currentTime += dt;

				Vector2 randomDirection = Vector2::RandomCireclePoint();
				data.magnitude -= data.decreaseFactor * dt;
				if (data.magnitude < 0) data.magnitude = 0;

				data.object->Move(randomDirection * dt * data.magnitude);
				if (data.object->GetPosition().Distance(data.initialPosition) > data.maxRadius)
					data.object->SetPosition(data.initialPosition);
			}
			else
			{
				data.object->SetPosition(data.initialPosition);
				_effectDatas.erase(_effectDatas.begin() + i);
			}
		}

		return shaked;
	}
}
