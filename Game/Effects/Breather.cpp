#include "Breather.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	Breather* Breather::_instance = nullptr;
	std::mutex Breather::_mutex;

	BreathData::BreathData(ITransformable* object, const float& time, const Vector2& scale, const easing::EaseType& inEaseType, const easing::EaseType& outEaseType, const bool& isLooping) :
		EffectData(object, time, isLooping),
		in(true),
		initialScale(object->GetScale()),
		scale(scale),
		inEaseType(inEaseType),
		outEaseType(outEaseType)
	{}
	void BreathData::operator=(const BreathData& data)
	{
		currentTime = 0;
		scale = data.scale;
		inEaseType = data.inEaseType;
		outEaseType = data.outEaseType;
		time = data.time;
	}

	Breather::Breather() = default;
	Breather::~Breather() = default;

	Breather* Breather::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Breather();
		return _instance;
	}

	bool Breather::_UpdateEffects(const float& dt)
	{
		bool breathed = false;

		for (int i = 0; i < _effectDatas.size(); ++i)
		{
			BreathData& data = _effectDatas[i];
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
					data.object->SetScale(data.initialScale);
					_effectDatas.erase(_effectDatas.begin() + i);

					return false;
				}
			}

			breathed = true;
			const float t = data.currentTime / data.time;
			if (data.in) data.object->SetScale(data.initialScale + data.scale * Ease(data.inEaseType, t));
			else data.object->SetScale(data.initialScale + data.scale * Ease(data.outEaseType, 1 - t));
		}

		return breathed;
	}
}
