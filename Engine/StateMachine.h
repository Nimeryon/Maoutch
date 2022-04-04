#pragma once
namespace sf
{
	class RenderWindow;
}
#include <memory>
#include <stack>

namespace maoutch
{
	class State;
	typedef std::unique_ptr<State> StateRef;

	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void AddState(StateRef state, bool isReplacing = true);
		void RemoveState();

		void HandleStateUpdate();

		void ProcessObjectsAdding();
		void ProcessInputs();
		void EarlyUpdate(float dt);
		void Update(float dt);
		void FixedUpdate(float fixedDt);
		void Draw(sf::RenderWindow& window);
		void LateUpdate(float dt);

		StateRef& GetState();
	private:
		std::stack<StateRef> _states;
		StateRef _state;

		bool _isRemoving;
		bool _isAdding;
		bool _isReplacing;

		void _HandleRemoving();
		void _HandleAdding();
	};
}