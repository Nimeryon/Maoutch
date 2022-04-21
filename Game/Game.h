#pragma once
#include <SFML/Graphics.hpp>

#include "../Engine/StateMachine.h"
#include "../Engine/Assets.h"

namespace maoutch
{
	struct GameData
	{
		sf::RenderWindow window;
		sf::View letterBoxView;
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

		std::string _title;

		void _Run();
		void _ProcessInputs();
		void _UpdateLetterBoxView(int width, int height);
	};
}
