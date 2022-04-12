#include "imgui.h"
#include "GameState.h"
#include "../../Game/Objects/MatchGrid.h"
#include "../Assets.h"

namespace maoutch
{
	GameState::GameState() :
		_gridPosition1{ Assets::Config<float>("Window", "Width") / 4.f, Assets::Config<float>("Window", "Height") / 2.f },
		_gridPosition2{ Assets::Config<float>("Window", "Width") / 4.f * 3.f, Assets::Config<float>("Window", "Height") / 2.f },
		_rotation(0) {}
	GameState::~GameState()
	{
		delete _grid1;
		delete _grid2;
	}

	void GameState::Init()
	{
		_grid1 = new MatchGrid("small_grid", Vector2(_gridPosition1[0], _gridPosition1[1]));
		_grid2 = new MatchGrid("circle_grid", Vector2(_gridPosition2[0], _gridPosition2[1]));
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

		if (ImGui::SliderFloat("Grid 1 Position X", &_gridPosition1[0], 0, windowWidth))
			_grid1->SetPosition(Vector2(_gridPosition1[0], _gridPosition1[1]));
		if (ImGui::SliderFloat("Grid 1 Position Y", &_gridPosition1[1], 0, windowHeight))
			_grid1->SetPosition(Vector2(_gridPosition1[0], _gridPosition1[1]));

		if (ImGui::SliderFloat("Grid 2 Position X", &_gridPosition2[0], 0, windowWidth))
			_grid2->SetPosition(Vector2(_gridPosition2[0], _gridPosition2[1]));
		if (ImGui::SliderFloat("Grid 2 Position Y", &_gridPosition2[1], 0, windowHeight))
			_grid2->SetPosition(Vector2(_gridPosition2[0], _gridPosition2[1]));

		if (ImGui::SliderFloat("Grids Rotation", &_rotation, -360, 360))
		{
			_grid1->SetRotation(_rotation);
			_grid2->SetRotation(_rotation);
		}

		if (ImGui::Button("Reset Grid 1"))
			_grid1->StartReset();
		if (ImGui::Button("Reset Grid 2"))
			_grid2->StartReset();

		ImGui::End();
	}
}
