#include "Timer.h"

namespace maoutch
{
	Event<float> TimerBase::timerEvent;

	TimerBase::TimerBase(float time) :
		_currentTime(0),
		_time(time)
	{}
	TimerBase::~TimerBase()
	{
		Stop();
	}

	void TimerBase::Start()
	{
		timerEvent += EventHandler::Bind<float, TimerBase>(&TimerBase::Update, this);
	}
	void TimerBase::Stop()
	{
		timerEvent -= EventHandler::Bind<float, TimerBase>(&TimerBase::Update, this);
	}
	void TimerBase::Reset()
	{
		Stop();
		_currentTime = 0;
	}

	void TimerBase::Update(float& dt)
	{
		_currentTime += dt;
		if (_currentTime >= _time)
		{
			_OnTimerEvent();
			Reset();
		}
	}

	template <typename T>
	Timer<T>::Timer(float time, void (T::* callback)(), T* instance) :
		TimerBase(time),
		_instance(instance),
		_callback(callback)
	{}

	template <typename T>
	void Timer<T>::_OnTimerEvent()
	{
		(_instance->*_callback)();
	}

	Timer<void>::Timer(float time, void (*callback)()) :
		TimerBase(time),
		_callback(callback)
	{}

	void Timer<void>::_OnTimerEvent()
	{
		_callback();
	}
}
