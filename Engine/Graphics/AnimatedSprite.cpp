#include <array>

#include "AnimatedSprite.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Tools/Texture.h"

namespace maoutch
{
	AnimatedSprite::AnimatedSprite(const float& animationSpeed, const sf::Texture* texture, const Vector2i& frameSize, const Vector2i& framePosition, const bool& playOnAwake) :
		_playOnAwake(playOnAwake),
		_currentFrame(0),
		_frameSize(frameSize),
		_frameTimer(animationSpeed, &AnimatedSprite::_OnFrameTimer, this, true)
	{
		_renderState.texture = texture;
		_vertexArray.setPrimitiveType(sf::Quads);

		_framePositions.emplace_back(framePosition);

		if (_playOnAwake) _frameTimer.Start();
	}
	AnimatedSprite::~AnimatedSprite() = default;

	float AnimatedSprite::GetAnimationSpeed() const { return _frameTimer.GetTime(); }
	int AnimatedSprite::GetFrameCount() const { return _framePositions.size(); }
	int AnimatedSprite::GetCurrentFrame() const { return _currentFrame; }
	bool AnimatedSprite::IsPlaying() const { return _frameTimer.IsPlaying(); }

	void AnimatedSprite::SetAnimationSpeed(const float& animationSpeed) { _frameTimer.SetTime(animationSpeed); }
	void AnimatedSprite::AddFrame(const Vector2i& framePosition) { _framePositions.emplace_back(framePosition); }
	void AnimatedSprite::Play()
	{
		_SetFrame();
		_frameTimer.Start();
	}
	void AnimatedSprite::Stop() { _frameTimer.Stop(); }

	void AnimatedSprite::Draw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_renderState.transform = transform;
		window.draw(_vertexArray, _renderState);
	}

	void AnimatedSprite::_SetFrame()
	{
		_vertexArray.clear();

		std::array<sf::Vertex, 4> vertices;
		texture::SetTextureCoord(vertices, _framePositions[_currentFrame], _frameSize);

		vertices[0].position = { 0, 0};
		vertices[1].position = { (float)_frameSize.x, 0};
		vertices[2].position = _frameSize;
		vertices[3].position = { 0, (float)_frameSize.y };

		for (sf::Vertex& vertex : vertices)
			_vertexArray.append(vertex);
	}
	void AnimatedSprite::_OnFrameTimer()
	{
		_currentFrame = ++_currentFrame % GetFrameCount();
		_SetFrame();
	}
}
