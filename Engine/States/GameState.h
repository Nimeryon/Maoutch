#pragma once
#include "../Graphics/Sprite.h"
#include "../../Interfaces/IStateDependant.h"
#include "State.h"

namespace maoutch
{
	class HealthBar;
	class MatchGrid;

	class GameState : public State, public IStateDependant
	{
	public:
		GameState();
		~GameState() override;

		void Init() override;
		void ProcessInputs() override;
		void Draw(sf::RenderWindow& window) override;
		void ImGui(float dt) override;

	private:
		Sprite* _background;

		void _OnStateChange() override;
	};
}