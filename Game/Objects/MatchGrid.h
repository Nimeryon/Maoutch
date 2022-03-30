#pragma once
#include "MatchElement.h"
#include "../../Types/Grid.h"
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Tools/Timer.h"
#include "../Match/MatchFinder.h"
#include "../../Types/Direction.h"

namespace maoutch
{
	const std::string gridPath = "Assets\\Grids\\";

	enum class GridState
	{
		MOVING,
		INPUTS
	};

	class MatchGrid : public GameObject
	{
	public:
		MatchGrid(const std::string& fileName, const Vector2& position);
		
		void FixedUpdate(float fixedDt) override;

		void Setup(const std::string& fileName);
		void SetupGrid(const Vector2i& gridSize);
		void SetupGridPos(const Vector2i& gridPos);

		bool IsValidGridPosition(const Vector2i& gridPos) const;
		bool ProcessMatches();

		void Reset();
		void DestroyMatched();
		void CollapseColumns();
		void RefillBoard();
		void AfterRefill();
		void Swap(const Vector2i gridPos, const Direction dir);
		void SwapBack();
		void UpdateElementsPosition();

		Vector2 GetCenterGridPosition(const Vector2i& gridPos) const;
		GridState GetState() const;
		void SetState(const GridState& state);

		void PrintGrid();

	private:
		Grid<MatchElement*> _grid;
		std::vector<Vector2i> _emptyPositions;
		GridState _state;
		MatchFinder _matchFinder;

		bool _moveChecked;
		bool _processAfterMoving;
		Vector2i _lastSwapGoalPos;
		Direction _lastSwapDir;

		Timer<MatchGrid> _destroyTimer;
		Timer<MatchGrid> _collapseTimer;
		Timer<MatchGrid> _refillTimer;
	};
}
