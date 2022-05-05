#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "GameState.h"
#include "../../Game/Match/Objects/MatchGrid.h"
#include "../../Game/Entities/Monster.h"
#include "../../Engine/StateMachine.h"
#include "../../Engine/InputHandler.h"
#include "../../Engine/States/ParticleEditorState.h"
#include "../Assets.h"

namespace maoutch
{
	GameState::GameState() : _background(
		Assets::Instance()->GetTexture("BackGround"),
		Vector2i(1280, 720),
		Vector2i::Zero()
	)
	{}
	GameState::~GameState()
	{
		delete _grid;
	}

	void GameState::Init()
	{
		_grid = new MatchGrid("circle_grid");
		new Monster(random::Bool() ? "cerbere" : "demon", Element::Random());
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
		_background.Draw(window, sf::Transform::Identity);
	}
	void GameState::ImGui(float dt)
	{
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
