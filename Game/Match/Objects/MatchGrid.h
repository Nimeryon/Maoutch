#pragma warning( disable : 4267 )
#pragma once
#include "MatchElement.h"
#include "../../Types/Grid.h"
#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/Tools/Timer.h"
#include "../MatchFinder.h"
#include "../../Types/Direction.h"
#include "../../Tools/Easing.h"

namespace maoutch
{
	class MatchGridBackGround;

	enum class GridState
	{
		Starting,
		Moving,
		Inputs,
		Disabled
	};

	class MatchGrid : public GameObject
	{
	public:
		MatchGrid(const std::string& fileName);
		
		void Setup(const std::string& fileName);
		void SetupGrid(const Vector2i& gridSize);
		void SetupGridPos(const Vector2i& gridPos);

		bool IsMoving() const;
		bool IsValidGridPosition(const Vector2i& gridPos) const;

		void StartReset();

		void Swap(const Vector2i gridPos, const Direction dir);
		void DestroyGridPos(const Vector2i& gridPos);

		void UpdateElementsPosition(const float& minStartMoveTime, const float& maxStartMoveTime, const easing::EaseType& easeType = easing::EaseType::None);
		void UpdateElementsPosition(const float& moveTime = 0, const easing::EaseType& easeType = easing::EaseType::None);

		Vector2 GetGlobalCenterGridPosition(const Vector2& gridPos) const;
		Vector2 GetCenterGridPosition(const Vector2& gridPos) const;
		GridState GetState() const;
		void SetState(const GridState& state);

		Grid<MatchElement*> GetGrid() const;

		void PrintGrid();

	private:
		MatchGridBackGround* _matchGridBackGround;
		Grid<MatchElement*> _grid;

		std::vector<Vector2i> _emptyPositions;
		std::vector<Vector2> _respawnPoints;
		GridState _state;
		MatchFinder _matchFinder;

		int _combo;
		std::vector<sf::Color> _comboColors;

		bool _moveChecked;
		Vector2i _lastSwapGoalPos;
		Direction _lastSwapDir;

		Vector2 _particleScale;

		float _shakeMinTime;
		float _shakeMaxTime;
		float _shakeMaxRadius;
		float _shakeMagnitude;
		float _shakeDecreaseFactor;

		Timer<MatchGrid> _startTimer;
		Timer<MatchGrid> _destroyTimer;
		Timer<MatchGrid> _collapseTimer;
		Timer<MatchGrid> _refillTimer;
		Timer<MatchGrid> _afterRefillTimer;
		Timer<MatchGrid> _startResetTimer;
		Timer<MatchGrid> _endResetTimer;
		Timer<MatchGrid> _swapBackTimer;
		Timer<MatchGrid> _processMatchTimer;
		Timer<MatchGrid> _showHintTimer;

		void _ResetCombo();
		void _AddCombo();

		void _DestroyMatched();
		void _CollapseColumns();
		void _RefillBoard();
		void _AfterRefill();
		void _Reset();

		void _ProcessMatches();
		void _ProcessPossibleMatches();

		void _Start();
		void _DisableMatchHint();
		void _SetPossibleMatch();
		void _SwapBack();
		void _FillGrid(bool createAtCenter = false);

		void _SpawnParticle(const Element& element, const Vector2i& gridPos);
		void _SpawnParticle(const std::string& fileName, const Vector2& position);
		void _SpawnComboPopUp(Match& match, const int& comboOffset);
	};
}
