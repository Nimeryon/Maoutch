#include <SFML/Graphics/RenderWindow.hpp>

#include "StateMachine.h"
#include "States/State.h"
#include "Tools/Timer.h"
#include "Objects/GameObjectHandler.h"

namespace maoutch
{
	StateMachine* StateMachine::_instance = nullptr;
	std::mutex StateMachine::_mutex;

	Event<void> StateMachine::stateChange;
	
	StateMachine* StateMachine::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new StateMachine();
		return _instance;
	}

	StateMachine::StateMachine() :
		_isSetting(false),
		_state(nullptr)
	{}
	StateMachine::~StateMachine() = default;

	void StateMachine::SetState(StateRef state)
	{
		_isSetting = true;
		_state = std::move(state);
	}

	void StateMachine::HandleStateUpdate()
	{
		_HandleStateChange();
	}

	void StateMachine::ProcessObjectsAdding()
	{
		GameObjectHandler::Instance()->ProcessObjectsAdding();
	}
	void StateMachine::ProcessInputs()
	{
		GetState()->ProcessInputs();
		GameObjectHandler::Instance()->ProcessInputs();
	}
	void StateMachine::EarlyUpdate(float dt)
	{
		TimerBase::timerEvent(dt);

		GetState()->EarlyUpdate(dt);
		GameObjectHandler::Instance()->EarlyUpdate(dt);
	}
	void StateMachine::Update(float dt)
	{
		GetState()->Update(dt);
		GameObjectHandler::Instance()->Update(dt);
	}
	void StateMachine::FixedUpdate(float fixedDt)
	{
		GetState()->FixedUpdate(fixedDt);
		GameObjectHandler::Instance()->FixedUpdate(fixedDt);
	}
	void StateMachine::Draw(sf::RenderWindow& window)
	{
		GetState()->Draw(window);
		GameObjectHandler::Instance()->Draw(window);
	}
	void StateMachine::LateUpdate(float dt)
	{
		GetState()->LateUpdate(dt);
		GameObjectHandler::Instance()->LateUpdate(dt);
	}

	StateRef& StateMachine::GetState() { return _states.top(); }
	
	void StateMachine::_HandleStateChange()
	{
		if (!_isSetting) return;

		stateChange();

		_states.push(std::move(_state));
		_states.top()->Init();
		_isSetting = false;
	}
}