#pragma once
#include "../../Tools/Event.h"

namespace maoutch
{
	struct TimerBase
	{
		TimerBase(float time) :
			_started(false),
			_currentTime(0),
			_time(time)
		{}
		virtual ~TimerBase() { Stop(); }

		TimerBase& operator=(const TimerBase&) = delete;

		void SetTime(const float& time) { _time = time; }

		float GetTime() const { return _time; }

		void Start()
		{
			if (_started) return;

			_started = true;
			timerEvent += EventHandler::Bind<float, TimerBase>(&TimerBase::Update, this);
		}
		void Stop()
		{
			_started = false;
			timerEvent -= EventHandler::Bind<float, TimerBase>(&TimerBase::Update, this);
		}
		void Reset()
		{
			Stop();
			_currentTime = 0;
		}

		void Update(float& dt)
		{
			_currentTime += dt;
			if (_currentTime >= _time)
			{
				Reset();
				_OnTimerEvent();
			}
		}

		static Event<float> timerEvent;

	private:
		virtual void _OnTimerEvent() = 0;

		bool _started;
		float _currentTime;
		float _time;
	};

	template <typename T>
	struct Timer : TimerBase
	{
		Timer(float time, void(T::* callback)(), T* instance) :
			TimerBase(time),
			_instance(instance),
			_callback(callback)
		{}
		Timer& operator=(const Timer&) = delete;

	private:
		void _OnTimerEvent() override
		{
			(_instance->*_callback)();
		}

		T* _instance;
		void(T::* _callback)();
	};

	template<>
	struct Timer<void> : TimerBase
	{
		Timer(float time, void(*callback)()) :
			TimerBase(time),
			_callback(callback)
		{}
		Timer& operator=(const Timer&) = delete;

	private:
		void _OnTimerEvent() override
		{
			_callback();
		}

		void(*_callback)();
	};
}
