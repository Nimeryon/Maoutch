#pragma once
#include "MatchElement.h"
#include "../../Types/Grid.h"
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Tools/Timer.h"
#include "../Match/MatchFinder.h"

namespace maoutch
{
	class Direction;
	enum class GridState
	{
		MOVING,
		INPUTS
	};

	class MatchGrid : public GameObject
	{
	public:
		// Minimum size 5x5
		MatchGrid(const int& width, const int& height, const Vector2& position);
		~MatchGrid() override;

		void Update(float dt) override;
		void FixedUpdate(float fixedDt) override;

		bool IsValidGridPosition(const Vector2i& gridPos) const;
		Vector2 GetCenterGridPosition(const Vector2i& gridPos) const;

		bool ProcessMatches();

		void Setup();
		void SetupGridPos(const Vector2i& gridPos);
		void Reset();
		void PrintGrid();
		void DestroyMatched();
		void RefillBoard();
		void AfterRefill();
		void Swap(const Vector2i gridPos, const Direction dir);
		void CollapseColumns();
		void UpdateElementsPosition();

		GridState GetState() const;
		void SetState(const GridState& state);

	private:
		Vector2 _velocity;

		Grid<MatchElement*> _grid;
		GridState _state;
		MatchFinder _matchFinder;

		Timer<MatchGrid> _destroyTimer;
		Timer<MatchGrid> _collapseTimer;
		Timer<MatchGrid> _refillTimer;
	};
}
