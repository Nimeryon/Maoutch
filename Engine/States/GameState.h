#pragma once
#include "../Graphics/Sprite.h"
#include "State.h"

namespace maoutch
{
	class MatchGrid;

	class GameState : public State
	{
	public:
		GameState();
		~GameState() override;

		void Init() override;
		void ProcessInputs() override;
		void Draw(sf::RenderWindow& window) override;
		void ImGui(float dt) override;

	private:
		MatchGrid* _grid = nullptr;

		Sprite _background;
	};
}