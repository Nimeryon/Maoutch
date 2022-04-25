#pragma once
#include <vector>
#include "../../Tools/Event.h"
#include "../../Engine/Tools/Timer.h"
#include "../../Interfaces/IStateDependant.h"

namespace maoutch
{
	class ITransformable;

	struct EffectData
	{
		EffectData(ITransformable* object, const float& time, const bool& isLooping = false);

		ITransformable* object;
		float currentTime;
		float time;
		bool isLooping;

		void operator=(const EffectData& data);
	};

	template <typename T>
	class Effect : IStateDependant
	{
	public:
		void Apply(const T& data)
		{
			if (!data.object) return;
			if (!_isActive)
			{
				_isActive = true;
				TimerBase::timerEvent += EventHandler::Bind<const float&, Effect<T>>(&Effect<T>::_Update, this);
			}

			bool objectFound = false;
			for (EffectData& effectData : _effectDatas)
			{
				if (effectData.object == data.object)
				{
					effectData = data;
					objectFound = true;
				}
			}

			if (!objectFound) _effectDatas.push_back(data);
		}

	protected:
		std::vector<T> _effectDatas;

		void _Clear()
		{
			_effectDatas.clear();
			TimerBase::timerEvent -= EventHandler::Bind<const float&, Effect<T>>(&Effect<T>::_Update, this);
			_isActive = false;
		}

		void _OnStateChange() override
		{
			_Clear();
		}

	private:
		bool _isActive = false;

		void _Update(const float& dt)
		{
			if (!_UpdateEffects(dt))
			{
				TimerBase::timerEvent -= EventHandler::Bind<const float&, Effect<T>>(&Effect<T>::_Update, this);
				_isActive = false;
			}
		}

		/* Return if an effect has been updated */
		virtual bool _UpdateEffects(const float& dt) { return false; }
	};
}
