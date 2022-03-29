#include <SFML/Graphics/RenderWindow.hpp>

#include "StateMachine.h"
#include "States/State.h"
#include "Tools/Timer.h"

namespace maoutch
{
	StateMachine::StateMachine() :
		_isRemoving(false),
		_isAdding(false),
		_isReplacing(false)
	{}
	StateMachine::~StateMachine() = default;

	void StateMachine::AddState(StateRef state, bool isReplacing)
	{
		_isAdding = true;
		_isReplacing = isReplacing;

		_state = std::move(state);
	}
	void StateMachine::RemoveState()
	{
		_isRemoving = true;
	}

	void StateMachine::HandleStateUpdate()
	{
		_HandleRemoving();
		_HandleAdding();
	}

	void StateMachine::ProcessInputs()
	{
		GetState()->ProcessInputs();
		GetState()->objectHandler.ProcessInputs();
	}
	void StateMachine::EarlyUpdate(float dt)
	{
		TimerBase::timerEvent(dt);

		GetState()->EarlyUpdate(dt);
		GetState()->objectHandler.EarlyUpdate(dt);
	}
	void StateMachine::Update(float dt)
	{
		GetState()->Update(dt);
		GetState()->objectHandler.Update(dt);
	}
	void StateMachine::FixedUpdate(float fixedDt)
	{
		GetState()->FixedUpdate(fixedDt);
		GetState()->objectHandler.FixedUpdate(fixedDt);
	}
	void StateMachine::Draw(sf::RenderWindow& window)
	{
		GetState()->Draw(window);
		GetState()->objectHandler.Draw(window);
	}
	void StateMachine::LateUpdate(float dt)
	{
		GetState()->LateUpdate(dt);
		GetState()->objectHandler.LateUpdate(dt);
	}

	StateRef& StateMachine::GetState() { return _states.top(); }

	void StateMachine::_HandleRemoving()
	{
		if (!_isRemoving || _states.empty()) return;

		_states.pop();
		if (!_states.empty()) _states.top()->Resume();
		_isRemoving = false;
	}
	void StateMachine::_HandleAdding()
	{
		if (!_isAdding) return;

		if (!_states.empty())
		{
			if (_isReplacing) _states.pop();
			else _states.top()->Pause();
		}

		_states.push(std::move(_state));
		_states.top()->Init();
		_isAdding = false;
	}
}