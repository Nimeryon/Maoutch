#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "GameState.h"
#include "../../Game/Objects/MatchGrid.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

namespace maoutch
{
	GameState::GameState() : _gridPosition{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f}, _timeSinceInit(0) {}
	GameState::~GameState()
	{
		delete _grid;
	}

	void GameState::Init()
	{
		_grid = new MatchGrid(16, 9, Vector2(_gridPosition[0], _gridPosition[1]));
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

		if (ImGui::SliderFloat("Grid Position X", &_gridPosition[0], 0, WINDOW_WIDTH))
			_grid->SetPosition(Vector2(_gridPosition[0], _gridPosition[1]));
		if (ImGui::SliderFloat("Grid Position Y", &_gridPosition[1], 0, WINDOW_HEIGHT))
			_grid->SetPosition(Vector2(_gridPosition[0], _gridPosition[1]));

		if (ImGui::Button("Reset Grid"))
			_grid->Reset();

		ImGui::End();
	}
}
