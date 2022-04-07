#pragma once
#include "MatchElement.h"
#include "../../Types/Grid.h"
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Tools/Timer.h"
#include "../Match/MatchFinder.h"
#include "../../Types/Direction.h"

namespace maoutch
{
	class MatchGridBackGround;

	static const std::string gridPath = "Assets\\Grids\\";

	static constexpr float destroyTime = .2f;
	static constexpr float collapseTime = .15f;
	static constexpr float refillTime = .25f;
	static constexpr float startResetTime = .5f;
	static constexpr float endResetTime = .5f;
	static constexpr float swapBackTime = .2f;
	static constexpr float processMatchTime = .33f;
	static constexpr float processPossibleMatchTime = .33f;
	static constexpr float showHintTime = 8.f;
	static constexpr float setupMinFallTime = 0.f;
	static constexpr float setupMaxFallTime = 1.75f;
	static constexpr float resetMinMoveTime = 0.f;
	static constexpr float resetMaxMoveTime = .5f;

	enum class GridState
	{
		STARTING,
		MOVING,
		INPUTS
	};

	class MatchGrid : public GameObject
	{
	public:
		MatchGrid(const std::string& fileName, const Vector2& position);

		void FixedUpdate(float dt) override;

		void Setup(const std::string& fileName);
		void SetupGrid(const Vector2i& gridSize);
		void SetupGridPos(const Vector2i& gridPos);

		bool IsMoving() const;
		bool IsValidGridPosition(const Vector2i& gridPos) const;

		void StartReset();

		void Swap(const Vector2i gridPos, const Direction dir);
		void DestroyGridPos(const Vector2i& gridPos);

		void UpdateElementsPosition(const float& minStartMoveTime, const float& maxStartMoveTime);
		void UpdateElementsPosition(const float& moveTime = 0);

		Vector2 GetCenterGridPosition(const Vector2i& gridPos) const;
		GridState GetState() const;
		void SetState(const GridState& state);

		void PrintGrid();

	private:
		MatchGridBackGround* _matchGridBackGround;
		Grid<MatchElement*> _grid;

		std::vector<Vector2i> _emptyPositions;
		GridState _state;
		MatchFinder _matchFinder;

		bool _moveChecked;
		Vector2i _lastSwapGoalPos;
		Direction _lastSwapDir;
		
		Timer<MatchGrid> _destroyTimer;
		Timer<MatchGrid> _collapseTimer;
		Timer<MatchGrid> _refillTimer;
		Timer<MatchGrid> _startResetTimer;
		Timer<MatchGrid> _endResetTimer;
		Timer<MatchGrid> _swapBackTimer;
		Timer<MatchGrid> _processMatchTimer;
		Timer<MatchGrid> _processPossibleMatchTimer;
		Timer<MatchGrid> _showHintTimer;

		void _DestroyMatched();
		void _CollapseColumns();
		void _RefillBoard();
		void _AfterRefill();
		void _Reset();

		void _ProcessMatches();
		void _ProcessPossibleMatches();

		void _DisableMatchHint();
		void _SetPossibleMatch();
		void _SwapBack();
		void _FillGrid(bool createAtCenter = false);
		void _SpawnParticle(const Element& element, const Vector2i& gridPos);
		void _SpawnParticle(const std::string& fileName, const Vector2& position);
	};
}
