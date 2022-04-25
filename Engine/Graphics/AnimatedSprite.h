#pragma warning( disable : 4267 )
#pragma once
namespace sf
{
	class RenderWindow;
}

#include <vector>

#include "Sprite.h"
#include "../Tools/Timer.h"
#include "../../Types/Vector2i.h"

namespace maoutch
{
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(const float& animationSpeed, const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition, const bool& playOnAwake = true, const bool& playOnce = false);
		~AnimatedSprite() override;

		float GetAnimationSpeed() const;
		int GetFrameCount() const;
		int GetCurrentFrame() const;
		bool IsPlaying() const;

		void SetAnimationSpeed(const float& animationSpeed);
		void AddFrame(const Vector2i& framePosition);
		void Play();
		void Stop();

	private:
		bool _playOnAwake;
		bool _playOnce;
		int _currentFrame;
		std::vector<Vector2i> _framePositions;

		Timer<AnimatedSprite> _frameTimer;

		void _SetFrame() override;
		void _OnFrameTimer();
	};
}
