#pragma once
namespace sf
{
	class RenderWindow;
}

#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

#include "../Tools/Timer.h"
#include "../../Types/Vector2i.h"

namespace maoutch
{
	class AnimatedSprite
	{
	public:
		AnimatedSprite(const float& animationSpeed, const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition, const bool& playOnAwake);
		~AnimatedSprite();

		float GetAnimationSpeed() const;
		int GetFrameCount() const;
		int GetCurrentFrame() const;
		bool IsPlaying() const;

		void SetAnimationSpeed(const float& animationSpeed);
		void AddFrame(const Vector2i& framePosition);
		void Play();
		void Stop();

		void Draw(sf::RenderWindow& window, const sf::Transform& transform);

	private:
		sf::RenderStates _renderState;
		sf::VertexArray _vertexArray;

		bool _playOnAwake;
		int _currentFrame;
		std::vector<Vector2i> _framePositions;
		Vector2i _frameSize;

		Timer<AnimatedSprite> _frameTimer;

		void _SetFrame();
		void _OnFrameTimer();
	};
}
