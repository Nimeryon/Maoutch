#pragma once
#include "../../Tools/Event.h"

namespace maoutch
{
	struct TimerBase
	{
		TimerBase(float time);
		virtual ~TimerBase();

		TimerBase& operator=(const TimerBase&) = delete;

		void Start();
		void Stop();
		void Reset();

		void Update(float& dt);

		static Event<float> timerEvent;

	private:
		virtual void _OnTimerEvent() = 0;
		
		float _currentTime;
		float _time;
	};

	template <typename T>
	struct Timer : TimerBase
	{
		Timer(float time, void(T::* callback)(), T* instance);
		Timer& operator=(const Timer&) = delete;

	private:
		void _OnTimerEvent() override;

		T* _instance;
		void(T::* _callback)();
	};

	template<>
	struct Timer<void> : TimerBase
	{
		Timer(float time, void(*callback)());
		Timer& operator=(const Timer&) = delete;

	private:
		void _OnTimerEvent() override;

		void(*_callback)();
	};
}
