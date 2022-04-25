#pragma once
#include <SFML/Graphics/Sprite.hpp>

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
		sf::Sprite _background;
	};
}