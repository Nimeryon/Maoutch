#include "Effect.h"

namespace maoutch
{
	EffectData::EffectData(ITransformable* object, const float& time, const bool& isLooping) :
		object(std::weak_ptr(object->shared_from_this())),
		currentTime(0),
		time(time),
		isLooping(isLooping)
	{}

	void EffectData::operator=(const EffectData& data)
	{
		currentTime = 0;
		time = data.time;
		isLooping = data.isLooping;
	}
}
