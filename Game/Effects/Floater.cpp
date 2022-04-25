#include "Floater.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	Floater* Floater::_instance = nullptr;
	std::mutex Floater::_mutex;

	FloaterData::FloaterData(ITransformable* object, const float& time, const float& height, const easing::EaseType& inEaseType, const easing::EaseType& outEaseType, const bool& isLooping) :
		EffectData(object, time, isLooping),
		in(true),
		initialHeight(object->GetPosition().y),
		height(height),
		inEaseType(inEaseType),
		outEaseType(outEaseType)
	{}
	void FloaterData::operator=(const FloaterData& data)
	{
		currentTime = 0;
		height = data.height;
		inEaseType = data.inEaseType;
		outEaseType = data.outEaseType;
		time = data.time;
	}

	Floater::Floater() = default;
	Floater::~Floater() = default;

	Floater* Floater::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Floater();
		return _instance;
	}

	bool Floater::_UpdateEffects(const float& dt)
	{
		bool floated = false;

		for (int i = 0; i < _effectDatas.size(); ++i)
		{
			FloaterData& data = _effectDatas[i];
			if (!data.object)
			{
				_effectDatas.erase(_effectDatas.begin() + i);
				continue;
			}

			data.currentTime += dt;
			if (data.currentTime >= data.time)
			{
				if (data.isLooping)
				{
					data.in = !data.in;
					data.currentTime -= data.time;
				}
				else
				{
					data.object->SetPosition(Vector2(data.object->GetPosition().x, data.initialHeight));
					_effectDatas.erase(_effectDatas.begin() + i);

					return false;
				}
			}

			floated = true;
			const float t = data.currentTime / data.time;
			if (data.in) data.object->SetPosition(Vector2(data.object->GetPosition().x, data.initialHeight + data.height * Ease(data.inEaseType, t)));
			else data.object->SetPosition(Vector2(data.object->GetPosition().x, data.initialHeight + data.height * Ease(data.outEaseType, 1 - t)));
		}

		return floated;
	}
}
