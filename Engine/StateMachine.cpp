#include <SFML/Graphics/RenderWindow.hpp>

#include "StateMachine.h"
#include "States/State.h"
#include "Tools/Timer.h"
#include "Objects/GameObjectHandler.h"

namespace maoutch
{
	StateMachine* StateMachine::_instance = nullptr;
	std::mutex StateMachine::_mutex;
	
	StateMachine* StateMachine::GetInstance()
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
		GameObjectHandler::GetInstance()->ProcessObjectsAdding();
	}
	void StateMachine::ProcessInputs()
	{
		GetState()->ProcessInputs();
		GameObjectHandler::GetInstance()->ProcessInputs();
	}
	void StateMachine::EarlyUpdate(float dt)
	{
		TimerBase::timerEvent(dt);

		GetState()->EarlyUpdate(dt);
		GameObjectHandler::GetInstance()->EarlyUpdate(dt);
	}
	void StateMachine::Update(float dt)
	{
		GetState()->Update(dt);
		GameObjectHandler::GetInstance()->Update(dt);
	}
	void StateMachine::FixedUpdate(float fixedDt)
	{
		GetState()->FixedUpdate(fixedDt);
		GameObjectHandler::GetInstance()->FixedUpdate(fixedDt);
	}
	void StateMachine::Draw(sf::RenderWindow& window)
	{
		GetState()->Draw(window);
		GameObjectHandler::GetInstance()->Draw(window);
	}
	void StateMachine::LateUpdate(float dt)
	{
		GetState()->LateUpdate(dt);
		GameObjectHandler::GetInstance()->LateUpdate(dt);
	}

	StateRef& StateMachine::GetState() { return _states.top(); }
	
	void StateMachine::_HandleStateChange()
	{
		if (!_isSetting) return;

		GameObjectHandler::GetInstance()->Clear();
		_states.push(std::move(_state));
		_states.top()->Init();
		_isSetting = false;
	}
}