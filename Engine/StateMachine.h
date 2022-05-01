#pragma once
namespace sf
{
	class RenderWindow;
}
#include <memory>
#include <stack>
#include <mutex>

#include "../../Tools/Event.h"

namespace maoutch
{
	class State;
	typedef std::unique_ptr<State> StateRef;

	class StateMachine
	{
	public:
		static StateMachine* Instance();

		StateMachine(StateMachine const&) = delete;
		void operator=(StateMachine const&) = delete;

		void SetState(StateRef state);

		void HandleStateUpdate();
		void ProcessObjectsAdding();
		void ProcessObjectsDestroy();

		void ProcessInputs();
		void EarlyUpdate(float dt);
		void Update(float dt);
		void FixedUpdate(float fixedDt);
		void Draw(sf::RenderWindow& window);
		void LateUpdate(float dt);

		StateRef& GetState();

		static Event<void> stateChange;

	protected:
		StateMachine();
		~StateMachine();

	private:
		static StateMachine* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		std::stack<StateRef> _states;
		StateRef _state;
		
		bool _isSetting;
		
		void _HandleStateChange();
	};
}