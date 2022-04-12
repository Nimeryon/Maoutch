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
		MatchGrid* _grid = nullptr;
		float _gridPosition[2];
		float _rotation;
	};
}