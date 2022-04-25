#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "GameState.h"
#include "../../Game/Objects/MatchGrid.h"
#include "../../Game/Entities/Monster.h"
#include "../../Engine/StateMachine.h"
#include "../../Engine/InputHandler.h"
#include "../../Engine/States/ParticleEditorState.h"
#include "../Assets.h"

namespace maoutch
{
	GameState::GameState()
	{
		_background.setTexture(Assets::Instance()->GetTexture("BackGround"));
		_background.setTextureRect({ 0, 0, 1280, 720 });
	}
	GameState::~GameState()
	{
		delete _grid;
	}

	void GameState::Init()
	{
		_grid = new MatchGrid("circle_grid", Element::ElementValue::Fire);

		const float windowWidth = Assets::Config<float>("Window", "Width");
		const float windowHeight = Assets::Config<float>("Window", "Height");

		Monster* monster = new Monster("demon_fire");
		monster->SetPosition(Vector2(windowWidth / 2.f, 150));
	}
	void GameState::ProcessInputs()
	{
		if (InputHandler::Instance()->IsKeyUp(sf::Keyboard::F6))
			StateMachine::Instance()->SetState(std::make_unique<ParticleEditorState>());

		if (InputHandler::Instance()->IsKeyUp(sf::Keyboard::F5))
			StateMachine::Instance()->SetState(std::make_unique<GameState>());
	}
	void GameState::Draw(sf::RenderWindow& window)
	{
		window.draw(_background);
	}
	void GameState::ImGui(float dt)
	{
		const float windowWidth = Assets::Config<float>("Window", "Width");
		const float windowHeight = Assets::Config<float>("Window", "Height");

		if (!ImGui::Begin("Game Infos"))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("FPS:");
		ImGui::SameLine();
		ImGui::Text(&std::to_string(1.f / dt)[0]);
		
		if (ImGui::Button("Reset Grid"))
			_grid->StartReset();

		ImGui::End();
	}
}
