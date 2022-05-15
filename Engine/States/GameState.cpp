#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "GameState.h"
#include "../../Game/Match/Objects/MatchGrid.h"
#include "../../Game/Entities/Monster.h"
#include "../../Engine/StateMachine.h"
#include "../../Engine/InputHandler.h"
#include "../../Engine/States/ParticleEditorState.h"
#include "../../Game/Entities/HealthBar.h"
#include "../Assets.h"

namespace maoutch
{
	GameState::GameState() : _background(nullptr) {}
	GameState::~GameState() = default;

	void GameState::Init()
	{
		// Background
		_background = new Sprite(
			Assets::Instance()->GetTexture("BackGround"),	
			Vector2i(1280, 720),
			Vector2i::Zero()
		);

		// Grid
		new MatchGrid("circle_grid");

		// HealthBar
		const float windowWidth = Assets::Config<float>("Window", "Width");
		HealthBar* healthBar = new HealthBar();
		healthBar->SetPosition(Vector2(windowWidth / 2.f, 64));

		// Monster
		Monster::CreateRandomMonster();
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
		_background->Draw(window);
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

		ImGui::End();
	}

	void GameState::_OnStateChange()
	{
		Monster::level = 0;
	}
}
