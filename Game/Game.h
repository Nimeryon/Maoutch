#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Engine/StateMachine.h"
#include "../Engine/AssetLoader.h"

namespace maoutch
{
	struct GameData
	{
		StateMachine stateMachine;
		sf::RenderWindow window;
		sf::Time logicDeltatime;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game
	{
	public:
		Game(std::string title);

	private:
		sf::Clock _clock;
		sf::Time _logicUpdateTime;
		GameDataRef _data = std::make_shared<GameData>();

		void _Run();
		void _ProcessInputs();
	};
}
