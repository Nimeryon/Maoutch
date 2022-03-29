#pragma once
#include <iostream>
#include <list>

namespace maoutch
{
	//=====================================================
	// # Event listener base
	//=====================================================
	template<typename T>
	class EventListenerBase
	{
	public:
		EventListenerBase() = default;
		virtual ~EventListenerBase() = default;

		virtual bool IsSameBindFunction(EventListenerBase<T>* elb) = 0;

		bool IsSameType(EventListenerBase<T>* elb)
		{
			return !(!elb || typeid(*this) != typeid(*elb));
		}
	};

	//=====================================================
	// # One argument event listener
	//=====================================================
	template<typename T>
	class EventListener : public EventListenerBase<T>
	{
	public:
		virtual void Invoke(T& value) = 0;
	};

	//=====================================================
	// # One argument event function
	//=====================================================
	template<typename T>
	class EventListenerFunction : public EventListener<T>
	{
	public:
		EventListenerFunction(void(*fn)(T& value)) : _function(fn) {}

		void Invoke(T& value) override { _function(value); }
		bool IsSameBindFunction(EventListenerBase<T>* el) override
		{
			if (!EventListenerBase<T>::IsSameType(el)) return false;

			const auto elb = dynamic_cast<EventListenerFunction<T>*>(el);
			if (!elb) return false;

			return this->_function == elb->_function;
		}

	private:
		void(*_function)(T& value);
	};

	//=====================================================
	// # One argument event for member function
	//=====================================================
	template<typename T, typename U>
	class EventListenerMemberFunction : public EventListener<T>
	{
	public:
		EventListenerMemberFunction(void(U::* fn)(T& value), U* instance) : _instance(instance), _function(fn) {}

		void Invoke(T& value) override { if (_instance) (_instance->*_function)(value); }
		bool IsSameBindFunction(EventListenerBase<T>* el) override
		{
			if (!EventListenerBase<T>::IsSameType(el)) return false;

			const auto elb = dynamic_cast<EventListenerMemberFunction<T, U>*>(el);
			if (!elb) return false;

			return this->_instance == elb->_instance && this->_function == elb->_function;
		}

	private:
		U* _instance;
		void(U::* _function)(T& value);
	};

	//=====================================================
	// # zero argument event listener
	//=====================================================
	template<>
	class EventListener<void> : public EventListenerBase<void>
	{
	public:
		virtual void Invoke() = 0;
	};

	//=====================================================
	// # zero argument event function
	//=====================================================
	template<>
	class EventListenerFunction<void> : public EventListener<void>
	{
	public:
		EventListenerFunction(void(*fn)()) : _function(fn) {}

		void Invoke() override { _function(); }
		bool IsSameBindFunction(EventListenerBase<void>* el) override
		{
			if (!IsSameType(el)) return false;

			const auto elb = dynamic_cast<EventListenerFunction<void>*>(el);
			if (!elb) return false;

			return this->_function == elb->_function;
		}

	private:
		void(*_function)();
	};

	//=====================================================
	// # zero argument for member function
	//=====================================================
	template<typename U>
	class EventListenerMemberFunction<void, U> : public EventListener<void>
	{
	public:
		EventListenerMemberFunction(void(U::* fn)(), U* instance) : _instance(instance), _function(fn) {}

		void Invoke() override { if (_instance) (_instance->*_function)(); }
		bool IsSameBindFunction(EventListenerBase<void>* el) override
		{
			if (!IsSameType(el)) return false;

			const auto elb = dynamic_cast<EventListenerMemberFunction<void, U>*>(el);
			if (!elb) return false;

			return this->_instance == elb->_instance && this->_function == elb->_function;
		}

	private:
		U* _instance;
		void(U::* _function)();
	};

	//=====================================================
	// # Event handler
	//=====================================================
	class EventHandler
	{
	public:
		template<typename T>
		static EventListener<T>* Bind(void(*fn)(T& value))
		{
			return new EventListenerFunction<T>(fn);
		}

		template<typename T, typename U>
		static EventListener<T>* Bind(void(U::* fn)(T& value), U* instance)
		{
			return new EventListenerMemberFunction<T, U>(fn, instance);
		}

		static EventListener<void>* Bind(void(*fn)())
		{
			return new EventListenerFunction<void>(fn);
		}

		template<typename U>
		static EventListener<void>* Bind(void(U::* fn)(), U* instance)
		{
			return new EventListenerMemberFunction<void, U>(fn, instance);
		}

		EventHandler() = delete;
	};


	//=====================================================
	// # Event base
	//=====================================================
	template<typename T>
	class EventBase
	{
	public:
		EventBase() = default;
		virtual ~EventBase() { ClearListeners(); }

		void Hook(EventListener<T>* el) { this += el; }
		void Unhook(EventListener<T>* el) { this -= el; }
		void ClearListeners()
		{
			for (auto listener : _listeners)
			{
				delete listener;
				listener = 0;
			}
			_listeners.clear();
		}

		EventBase& operator+=(EventListener<T>* el)
		{
			_listeners.push_back(el);

			return *this;
		}
		EventBase& operator-=(EventListener<T>* el)
		{
			for (auto i = _listeners.begin(); i != _listeners.end(); ++i)
			{
				EventListener<T>* listener = *i;
				if (el->IsSameBindFunction(listener))
				{
					delete listener;
					listener = 0;

					_listeners.erase(i);
					break;
				}
			}

			return *this;
		}

		// Disable assignment operator
		EventBase& operator=(const EventBase&) = delete;

	protected:
		std::list<EventListener<T>*> _listeners;
	};

	//=====================================================
	// # One argument event
	//=====================================================
	template<typename T>
	class Event : public EventBase<T>
	{
	public:
		void operator()(T& value)
		{
			auto listeners = this->_listeners;
			for (const auto listener : listeners)
				if (listener != nullptr) listener->Invoke(value);
		}
	};

	//=====================================================
	// # Zero argument event
	//=====================================================
	template<>
	class Event<void> : public EventBase<void>
	{
	public:
		void operator()()
		{
			auto listeners = _listeners;
			for (const auto listener : listeners)
				if (listener != nullptr) listener->Invoke();
		}
	};
}