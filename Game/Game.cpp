#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include "Game.h"

#include "../Engine/States/State.h"
#include "../Engine/States/ParticleEditorState.h"
#include "../Engine/States/GameState.h"
#include "../Engine/InputHandler.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

namespace maoutch
{
	Game::Game(std::string title)
	{
		_data->window.create(
			sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
			title, sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize
		);
		ImGui::SFML::Init(_data->window);
		InputHandler::GetInstance()->SetWindow(&_data->window);

		// Setup framerate
		_data->window.setFramerateLimit(240);

		// Setup logic time
		_data->logicDeltatime = sf::milliseconds(1000.f / 60.f);

		// Try initing assets
		if (!AssetLoader::GetInstance()->Init())
		{
			std::cout << "Loading error!\n";
			return;
		}

		// Add default state
		_data->stateMachine.AddState(std::make_unique<GameState>());
		_Run();
	}

	void Game::_Run()
	{
		while (_data->window.isOpen())
		{
			// Handle clock
			sf::Time time = _clock.restart();
			float renderDt = time.asSeconds();

			// Handle state update
			_data->stateMachine.HandleStateUpdate();
			_data->stateMachine.ProcessObjectsAdding();

			_ProcessInputs();

			// Handle ImGui update
			ImGui::SFML::Update(_data->window, time);
			
			_data->stateMachine.EarlyUpdate(renderDt);
			_data->stateMachine.Update(renderDt);

			// Handle fixed update
			_logicUpdateTime += time;
			if (_logicUpdateTime.asSeconds() > .1f) _logicUpdateTime = sf::milliseconds(100);
			while (_logicUpdateTime >= _data->logicDeltatime)
			{
				float logicDt = _logicUpdateTime.asSeconds();
				_data->stateMachine.FixedUpdate(logicDt);

				_logicUpdateTime -= _data->logicDeltatime;
			}

			// Handle ImGui call
			_data->stateMachine.GetState()->ImGui(renderDt);

			// Handle rendering
			_data->window.clear();


			_data->stateMachine.Draw(_data->window);
			ImGui::SFML::Render(_data->window);

			_data->window.display();

			// Handle late update
			_data->stateMachine.LateUpdate(renderDt);
		}
	}
	void Game::_ProcessInputs()
	{
		InputHandler::GetInstance()->ClearLastFrameEvents();

		sf::Event event;
		while (_data->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_data->window.close();

			ImGui::SFML::ProcessEvent(_data->window, event);
			InputHandler::GetInstance()->ProcessEvent(event);
		}

		_data->stateMachine.ProcessInputs();
	}
}
