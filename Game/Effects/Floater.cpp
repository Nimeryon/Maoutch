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

	bool Floater::_UpdateEffect(const float& dt, FloaterData& data, ITransformable* transformable)
	{
		data.currentTime += dt;
		if (data.currentTime >= data.time)
		{
			if (data.isLooping)
			{
				data.currentTime -= data.time;
				data.in = !data.in;
			}
			else
			{
				transformable->SetPosition(Vector2(transformable->GetPosition().x, data.initialHeight));
				return false;
			}
		}

		const float t = data.currentTime / data.time;
		if (data.in) transformable->SetPosition(Vector2(transformable->GetPosition().x, data.initialHeight + data.height * Ease(data.inEaseType, t)));
		else transformable->SetPosition(Vector2(transformable->GetPosition().x, data.initialHeight + data.height * Ease(data.outEaseType, 1 - t)));

		return true;
	}
}
