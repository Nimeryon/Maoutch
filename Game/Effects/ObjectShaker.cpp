#include "ObjectShaker.h"

#include "../../Engine/Objects/GameObject.h"
#include "../../Tools/Event.h"
#include "../../Engine/Tools/Timer.h"

namespace maoutch
{
	ObjectShaker* ObjectShaker::_instance = nullptr;
	std::mutex ObjectShaker::_mutex;

	ObjectShaker::ShakeData::ShakeData(GameObject* object, const float& time, const float& maxRadius, const float& magnitude, const float& decreaseFactor) :
		objectToShake(object),
		initialPosition(object->GetPosition()),
		maxRadius(maxRadius),
		magnitude(magnitude),
		decreaseFactor(decreaseFactor),
		currentTime(0),
		time(time)
	{}

	void ObjectShaker::ShakeData::operator=(const ShakeData& data)
	{
		currentTime = 0;
		maxRadius = data.maxRadius;
		magnitude = data.magnitude;
		decreaseFactor = data.decreaseFactor;
		time = data.time;
	}

	ObjectShaker::ObjectShaker() = default;
	ObjectShaker::~ObjectShaker() = default;

	ObjectShaker* ObjectShaker::_GetInstance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new ObjectShaker();
		return _instance;
	}

	void ObjectShaker::Shake(GameObject* object, const float& time, const float& maxRadius, const float& magnitude, const float& decreaseFactor)
	{
		if (!object) return;

		_GetInstance()->_AddObjectToShake(ShakeData(object, time, maxRadius, magnitude, decreaseFactor));
	}

	void ObjectShaker::_AddObjectToShake(ShakeData shakeData)
	{
		if (!_isShaking)
		{
			_isShaking = true;
			TimerBase::timerEvent += EventHandler::Bind<const float&, ObjectShaker>(&ObjectShaker::_Update, this);
		}

		bool objectFound = false;
		for (ShakeData& data : _objectsToShake)
		{
			if (data.objectToShake == shakeData.objectToShake)
			{
				data = shakeData;
				objectFound = true;
			}
		}

		if (!objectFound) _objectsToShake.push_back(shakeData);
	}
	void ObjectShaker::_Update(const float& dt)
	{
		bool shaked = false;

		for (int i = 0; i < _objectsToShake.size(); ++i)
		{
			ShakeData& data = _objectsToShake[i];
			if (!data.objectToShake)
			{
				_objectsToShake.erase(_objectsToShake.begin() + i);
				continue;
			}

			if (data.currentTime < data.time)
			{
				shaked = true;
				data.currentTime += dt;

				Vector2 randomDirection = Vector2::RandomCireclePoint();
				data.magnitude -= data.decreaseFactor * dt;
				if (data.magnitude < 0) data.magnitude = 0;

				data.objectToShake->Move(randomDirection * dt * data.magnitude);
				if (data.objectToShake->GetPosition().Distance(data.initialPosition) > data.maxRadius)
					data.objectToShake->SetPosition(data.initialPosition);
			}
			else
			{
				data.objectToShake->SetPosition(data.initialPosition);
				_objectsToShake.erase(_objectsToShake.begin() + i);
			}
		}

		if (!shaked)
		{
			TimerBase::timerEvent -= EventHandler::Bind<const float&, ObjectShaker>(&ObjectShaker::_Update, this);
			_isShaking = false;
		}
	}
}
