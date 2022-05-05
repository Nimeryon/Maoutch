#include "MatchHint.h"
#include "../MatchFinder.h"
#include "../Objects/MatchGrid.h"
#include "../../Engine/Assets.h"

namespace maoutch
{
	MatchHint::MatchHint() :
		GameObject("MatchHint", 2, true, false),
		_hintAnimationNorth(AnimatedSprite(
			Assets::Config<float>("Hint", "AnimationTime"),
			Assets::Instance()->GetTexture("Hint Arrow Animation"),
			Vector2i(Assets::Config<int>("Element", "Size"), Assets::Config<int>("Element", "Size") * 2),
			Vector2i(3, 0),
			false
		)),
		_hintAnimationEast(AnimatedSprite(
			Assets::Config<float>("Hint", "AnimationTime"),
			Assets::Instance()->GetTexture("Hint Arrow Animation"),
			Vector2i(Assets::Config<int>("Element", "Size") * 2, Assets::Config<int>("Element", "Size")),
			Vector2i::Zero(),
			false
		)),
		_hintAnimationSouth(AnimatedSprite(
			Assets::Config<float>("Hint", "AnimationTime"),
			Assets::Instance()->GetTexture("Hint Arrow Animation"),
			Vector2i(Assets::Config<int>("Element", "Size"), Assets::Config<int>("Element", "Size") * 2),
			Vector2i(2, 0),
			false
		)),
		_hintAnimationWest(AnimatedSprite(
			Assets::Config<float>("Hint", "AnimationTime"),
			Assets::Instance()->GetTexture("Hint Arrow Animation"),
			Vector2i(Assets::Config<int>("Element", "Size") * 2, Assets::Config<int>("Element", "Size")),
			Vector2i(0, 1),
			false
		))
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

		const float elementSize = Assets::Config<float>("Element", "Size");
		switch (_possibleMatch.direction)
		{
			case Direction::DirectionValue::North:
				SetOrigin(Vector2(elementSize / 2.f, elementSize + elementSize / 2.f));
				_hintAnimationNorth.Play();
				break;

			case Direction::DirectionValue::East:
				SetOrigin(Vector2(elementSize / 2.f, elementSize / 2.f));
				_hintAnimationEast.Play();
				break;

			case Direction::DirectionValue::South:
				SetOrigin(Vector2(elementSize / 2.f, elementSize / 2.f));
				_hintAnimationSouth.Play();
				break;

			case Direction::DirectionValue::West:
				SetOrigin(Vector2(elementSize + elementSize / 2.f, elementSize / 2.f));
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
		case Direction::DirectionValue::North:
			_hintAnimationNorth.Draw(window, transform);
			break;

		case Direction::DirectionValue::East:
			_hintAnimationEast.Draw(window, transform);
			break;

		case Direction::DirectionValue::South:
			_hintAnimationSouth.Draw(window, transform);
			break;

		case Direction::DirectionValue::West:
			_hintAnimationWest.Draw(window, transform);
			break;

		default: break;
		}
	}
}
