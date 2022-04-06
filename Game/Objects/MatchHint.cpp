#include "MatchHint.h"
#include "../Match/MatchFinder.h"
#include "../Objects/MatchGrid.h"
#include "../../Engine/AssetLoader.h"

extern maoutch::Vector2 ELEMENT_SIZE;

namespace maoutch
{
	MatchHint::MatchHint() :
		GameObject("MatchHint", 2, true, false),
		_hintAnimationNorth(
			animationTime,
			&AssetLoader::GetInstance()->GetTexture("Hint Arrow Animation"),
			Vector2i(ELEMENT_SIZE.x, ELEMENT_SIZE.y * 2),
			Vector2i(3, 0),
			false
		),
		_hintAnimationEast(
			animationTime,
			&AssetLoader::GetInstance()->GetTexture("Hint Arrow Animation"),
			Vector2i(ELEMENT_SIZE.x * 2, ELEMENT_SIZE.y),
			Vector2i::Zero(),
			false
		),
		_hintAnimationSouth(
			animationTime,
			&AssetLoader::GetInstance()->GetTexture("Hint Arrow Animation"),
			Vector2i(ELEMENT_SIZE.x, ELEMENT_SIZE.y * 2),
			Vector2i(2, 0),
			false
		),
		_hintAnimationWest(
			animationTime,
			&AssetLoader::GetInstance()->GetTexture("Hint Arrow Animation"),
			Vector2i(ELEMENT_SIZE.x * 2, ELEMENT_SIZE.y),
			Vector2i(0, 1),
			false
		)
	{
		// Setup Animation North Frames
		for (int i = 7; i < 32; i += 4)
			_hintAnimationNorth.AddFrame(Vector2i(i, 0));

		// Setup Animation East Frames
		for (int i = 2; i < 16; i += 2)
			_hintAnimationEast.AddFrame(Vector2i(i, 0));

		// Setup Animation South Frames
		for (int i = 6; i < 32; i += 4)
			_hintAnimationSouth.AddFrame(Vector2i(i, 0));

		// Setup Animation West Frames
		for (int i = 2; i < 16; i += 2)
			_hintAnimationWest.AddFrame(Vector2i(i, 1));
	}
	MatchHint::~MatchHint() = default;

	void MatchHint::SetActive(const bool& isActive)
	{
		GameObject::SetActive(isActive);

		if (!IsActive()) _StopAnimations();
	}
	void MatchHint::SetPossibleMatch(const PossibleMatch& possibleMatch)
	{
		_possibleMatch = possibleMatch;
		const auto grid = (MatchGrid*)parent;

		SetPosition(grid->GetCenterGridPosition(_possibleMatch.gridPos));

		_StopAnimations();

		switch (_possibleMatch.direction)
		{
			case Direction::DirectionValue::NORTH:
				SetOrigin(Vector2(ELEMENT_SIZE.x / 2.f, ELEMENT_SIZE.y + ELEMENT_SIZE.y / 2.f));
				_hintAnimationNorth.Play();
				break;

			case Direction::DirectionValue::EAST:
				SetOrigin(Vector2(ELEMENT_SIZE.x / 2.f, ELEMENT_SIZE.y / 2.f));
				_hintAnimationEast.Play();
				break;

			case Direction::DirectionValue::SOUTH:
				SetOrigin(Vector2(ELEMENT_SIZE.x / 2.f, ELEMENT_SIZE.y / 2.f));
				_hintAnimationSouth.Play();
				break;

			case Direction::DirectionValue::WEST:
				SetOrigin(Vector2(ELEMENT_SIZE.x + ELEMENT_SIZE.x / 2.f, ELEMENT_SIZE.y / 2.f));
				_hintAnimationWest.Play();
				break;

			default: break;
		}
	}

	void MatchHint::_StopAnimations()
	{
		_hintAnimationNorth.Stop();
		_hintAnimationEast.Stop();
		_hintAnimationSouth.Stop();
		_hintAnimationWest.Stop();
	}
	void MatchHint::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		switch (_possibleMatch.direction)
		{
		case Direction::DirectionValue::NORTH:
			_hintAnimationNorth.Draw(window, transform);
			break;

		case Direction::DirectionValue::EAST:
			_hintAnimationEast.Draw(window, transform);
			break;

		case Direction::DirectionValue::SOUTH:
			_hintAnimationSouth.Draw(window, transform);
			break;

		case Direction::DirectionValue::WEST:
			_hintAnimationWest.Draw(window, transform);
			break;

		default: break;
		}
	}
}
