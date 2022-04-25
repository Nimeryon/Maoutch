#include "IStateDependant.h"
#include "../../Tools/Event.h"
#include "../../Engine/StateMachine.h"

namespace maoutch
{
	IStateDependant::IStateDependant()
	{
		StateMachine::stateChange += EventHandler::Bind(&IStateDependant::_OnStateChange, this);
	}
	IStateDependant::~IStateDependant()
	{
		StateMachine::stateChange -= EventHandler::Bind(&IStateDependant::_OnStateChange, this);
	}
}
