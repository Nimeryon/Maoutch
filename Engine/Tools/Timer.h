#pragma once
#include "../../Tools/Event.h"

namespace maoutch
{
	struct TimerBase
	{
		explicit TimerBase(const float& time, const bool& looping) :
			_started(false),
			_looping(looping),
			_currentTime(0),
			_time(time)
		{}
		virtual ~TimerBase() { Stop(); }

		TimerBase& operator=(const TimerBase&) = delete;

		void SetTime(const float& time) { _time = time; }
		void SetLooping(const bool& looping) { _looping = looping; }

		float GetTime() const { return _time; }
		bool IsPlaying() const { return _started; }

		void Start()
		{
			if (_started) return;

			_started = true;
			timerEvent += EventHandler::Bind<float, TimerBase>(&TimerBase::Update, this);
		}
		void Restart()
		{
			Reset();
			Start();
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
				_OnTimerEvent();

				if (!_looping) Reset();
				else _currentTime -= _time;
			}
		}

		static Event<float> timerEvent;

	private:
		virtual void _OnTimerEvent() = 0;

		bool _started;
		bool _looping;
		float _currentTime;
		float _time;
	};

	template <typename T>
	struct Timer : TimerBase
	{
		Timer(float time, void(T::* callback)(), T* instance, const bool& looping = false) :
			TimerBase(time, looping),
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
		Timer(float time, void(*callback)(), const bool& looping = false) :
			TimerBase(time, looping),
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
