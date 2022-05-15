#pragma once
#include <vector>
#include "../../Tools/Event.h"
#include "../../Engine/Objects/GameObjectHandler.h"
#include "../../Engine/Tools/Timer.h"
#include "../../Interfaces/IStateDependant.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	struct EffectData
	{
		EffectData(ITransformable* object, const float& time, const bool& isLooping = false);

		std::weak_ptr<ITransformable> object;
		float currentTime;
		float time;
		bool isLooping;

		void operator=(const EffectData& data);
	};

	template <typename T>
	class Effect : IStateDependant
	{
	public:
		Effect()
		{
			GameObjectHandler::Instance()->onITransformableDestroy += EventHandler::Bind<ITransformable*, Effect>(&Effect::Remove, this);
		}
		~Effect() override
		{
			GameObjectHandler::Instance()->onITransformableDestroy -= EventHandler::Bind<ITransformable*, Effect>(&Effect::Remove, this);
		}

		void Apply(T data)
		{
			std::shared_ptr<ITransformable> transform = data.object.lock();
			if (!transform) return;
			if (!_isActive)
			{
				_isActive = true;
				TimerBase::timerEvent += EventHandler::Bind<const float&, Effect>(&Effect::_Update, this);
			}

			bool objectFound = false;
			for (T& effectData : _effectDatas)
			{
				std::shared_ptr<ITransformable> dataTransform = data.object.lock();
				if (dataTransform && dataTransform == transform)
				{
					effectData = data;
					objectFound = true;
				}
			}

			if (!objectFound) _effectDatas.push_back(data);
		}
		void Remove(ITransformable*& object)
		{
			if (!_isActive) return;
			
			_effectDatas.erase(std::remove_if(
				_effectDatas.begin(),
				_effectDatas.end(),
				[this, object](T& data)
				{
					std::shared_ptr<ITransformable> transform = data.object.lock();
					if (transform)
						return transform.get() == object;

					return false;
				}
			), _effectDatas.end());

			if (_effectDatas.empty())
			{
				TimerBase::timerEvent -= EventHandler::Bind<const float&, Effect<T>>(&Effect<T>::_Update, this);
				_isActive = false;
			}
		}

	protected:
		std::vector<T> _effectDatas;

		void _Clear()
		{
			_effectDatas.clear();
			TimerBase::timerEvent -= EventHandler::Bind<const float&, Effect>(&Effect::_Update, this);
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
		
		bool _UpdateEffects(const float& dt)
		{
			for (int i = 0; i < _effectDatas.size(); ++i)
			{
				std::shared_ptr<ITransformable> transform = _effectDatas[i].object.lock();
				if (transform)
				{
					if (!_UpdateEffect(dt, _effectDatas[i], transform.get()))
					{
						ITransformable* transformable = transform.get();
						Remove(transformable);
					}
				}
				else _effectDatas.erase(_effectDatas.begin() + i);
			}

			return !_effectDatas.empty();
		}
		virtual bool _UpdateEffect(const float& dt, T& data, ITransformable* transformable) = 0;
	};
}
