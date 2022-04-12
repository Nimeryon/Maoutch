#include "imgui.h"
#include "GameState.h"
#include "../../Game/Objects/MatchGrid.h"
#include "../Assets.h"

namespace maoutch
{
	GameState::GameState() :
		_gridPosition{ Assets::Config<float>("Window", "Width") / 2.f, Assets::Config<float>("Window", "Height") / 2.f },
		_rotation(0) {}
	GameState::~GameState()
	{
		delete _grid;
	}

	void GameState::Init()
	{
		_grid = new MatchGrid("small_grid", Vector2(_gridPosition[0], _gridPosition[1]));
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

		if (ImGui::SliderFloat("Grid Position X", &_gridPosition[0], 0, windowWidth))
			_grid->SetPosition(Vector2(_gridPosition[0], _gridPosition[1]));
		if (ImGui::SliderFloat("Grid Position Y", &_gridPosition[1], 0, windowHeight))
			_grid->SetPosition(Vector2(_gridPosition[0], _gridPosition[1]));
		
		if (ImGui::SliderFloat("Grids Rotation", &_rotation, -360, 360))
			_grid->SetRotation(_rotation);

		if (ImGui::Button("Reset Grid"))
			_grid->StartReset();

		ImGui::End();
	}
}
