#include <imgui-SFML.h>

#include "Game.h"

#include "../Engine/StateMachine.h"
#include "../Engine/States/State.h"
#include "../Engine/States/ParticleEditorState.h"
#include "../Engine/States/GameState.h"
#include "../Engine/InputHandler.h"

namespace maoutch
{
	Game::Game(std::string title) :
		_title(title)
	{
		// Try initing assets
		if (!Assets::Instance()->Init())
		{
			std::cout << "Loading error!\n";
			return;
		}

		const float windowWidth = Assets::Config<float>("Window", "Width");
		const float windowHeight = Assets::Config<float>("Window", "Height");
		_data->window.create(
			sf::VideoMode(windowWidth, windowHeight, 1),
			_title
		);
		_data->window.setActive(false);

		_data->letterBoxView.setSize(windowWidth, windowHeight);
		_data->letterBoxView.setCenter(windowWidth / 2, windowHeight / 2);
		_data->window.setView(_data->letterBoxView);

		ImGui::SFML::Init(_data->window);
		InputHandler::Instance()->SetWindow(&_data->window);

		// Setup framerate
		_data->window.setFramerateLimit(240);

		// Setup logic time
		_data->logicDeltatime = sf::milliseconds(1000.f / 60.f);

		// Add default state
		StateMachine::Instance()->SetState(std::make_unique<GameState>());
		
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
			StateMachine::Instance()->HandleStateUpdate();
			StateMachine::Instance()->ProcessObjectsAdding();

			_ProcessInputs();

			// Handle ImGui update
			ImGui::SFML::Update(_data->window, time);

			StateMachine::Instance()->EarlyUpdate(renderDt);
			StateMachine::Instance()->Update(renderDt);

			// Handle fixed update
			_logicUpdateTime += time;
			if (_logicUpdateTime.asSeconds() > .1f) _logicUpdateTime = sf::milliseconds(100);
			while (_logicUpdateTime >= _data->logicDeltatime)
			{
				float logicDt = _logicUpdateTime.asSeconds();
				StateMachine::Instance()->FixedUpdate(logicDt);

				StateMachine::Instance()->ProcessObjectsAdding();
				StateMachine::Instance()->ProcessObjectsDestroy();

				_logicUpdateTime -= _data->logicDeltatime;
			}

			// Handle ImGui call
			StateMachine::Instance()->GetState()->ImGui(renderDt);

			// Handle rendering
			_data->window.clear();
			StateMachine::Instance()->Draw(_data->window);
			ImGui::SFML::Render(_data->window);
			_data->window.display();

			// Handle late update
			StateMachine::Instance()->LateUpdate(renderDt);
			StateMachine::Instance()->ProcessObjectsDestroy();
		}
	}
	void Game::_ProcessInputs()
	{
		InputHandler::Instance()->ClearLastFrameEvents();

		sf::Event event;
		while (_data->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_data->window.close();

			if (event.type == sf::Event::Resized)
			{
				_UpdateLetterBoxView(event.size.width, event.size.height);
				_data->window.setView(_data->letterBoxView);
			}

			ImGui::SFML::ProcessEvent(_data->window, event);
			InputHandler::Instance()->ProcessEvent(event);
		}
		StateMachine::Instance()->ProcessInputs();
	}
	void Game::_UpdateLetterBoxView(int width, int height)
	{
		const float aspectRatio = (float)width / (float)height;
		const float viewRatio = (float)_data->letterBoxView.getSize().x / (float)_data->letterBoxView.getSize().y;
		float sizeX = 1;
		float sizeY = 1;
		float posX = 0;
		float posY = 0;

		bool horizontalSpacing = true;
		if (aspectRatio < viewRatio)
			horizontalSpacing = false;
		
		if (horizontalSpacing) {
			sizeX = viewRatio / aspectRatio;
			posX = (1 - sizeX) / 2.f;
		}
		else {
			sizeY = aspectRatio / viewRatio;
			posY = (1 - sizeY) / 2.f;
		}
		_data->letterBoxView.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	}
}
