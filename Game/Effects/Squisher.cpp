#include "Squisher.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	Squisher* Squisher::_instance = nullptr;
	std::mutex Squisher::_mutex;

	SquishData::SquishData(ITransformable* object, const float& time, const float& timeOut, const Vector2& scale, const easing::EaseType& inEaseType, const easing::EaseType& outEaseType, const bool& isLooping) :
		EffectData(object, time, isLooping),
		in(false),
		currentOutTime(0),
		timeOut(timeOut),
		initialScale(object->GetScale()),
		scale(scale),
		inEaseType(inEaseType),
		outEaseType(outEaseType)
	{}

	void SquishData::operator=(const SquishData& data)
	{
		currentOutTime = 0;
		currentTime = 0;
		in = false;
		scale = data.scale;
		inEaseType = data.inEaseType;
		outEaseType = data.outEaseType;
		time = data.time;
		timeOut = data.timeOut;
	}

	Squisher::Squisher() = default;
	Squisher::~Squisher() = default;

	Squisher* Squisher::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Squisher();
		return _instance;
	}
	
	bool Squisher::_UpdateEffect(const float& dt, SquishData& data, ITransformable* transformable)
	{
		bool isFinished = false;
		if (data.in)
		{
			data.currentTime += dt;
			if (data.currentTime >= data.time)
			{
				if (data.isLooping)
				{
					data.in = !data.in;
					data.currentTime -= data.time;
				}
				else isFinished = true;
			}

			const float t = data.currentTime / data.time;
			transformable->SetScale(data.initialScale + data.scale * Ease(data.inEaseType, t));
		}
		else
		{
			data.currentOutTime += dt;
			if (data.currentOutTime >= data.timeOut)
			{
				if (data.isLooping)
				{
					data.in = !data.in;
					data.currentOutTime -= data.timeOut;
				}
				else isFinished = true;
			}

			const float t = data.currentOutTime / data.timeOut;
			transformable->SetScale(data.initialScale + data.scale * Ease(data.outEaseType, 1 - t));
		}

		if (isFinished)
		{
			transformable->SetScale(data.initialScale);
			return false;
		}

		return true;
	}
}
