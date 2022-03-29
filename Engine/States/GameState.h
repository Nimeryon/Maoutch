#pragma once
#include "State.h"

namespace maoutch
{
	class MatchGrid;

	class GameState : public State
	{
	public:
		GameState();
		~GameState() override;

		void Init() override;
		void ImGui(float dt) override;

	private:
		MatchGrid* _grid1 = nullptr;
		MatchGrid* _grid2 = nullptr;
		float _gridPosition1[2];
		float _gridPosition2[2];
		float _rotation;
	};
}