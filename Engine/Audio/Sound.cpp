#include <SFML/Audio/SoundBuffer.hpp>

#include "Sound.h"

namespace maoutch
{
	Sound::Sound(const sf::SoundBuffer& buffer, const float& pitch) :
		_sound(sf::Sound(buffer))
	{
		SetPitch(pitch);
	}
	Sound::~Sound() = default;

	void Sound::Play() { _sound.play(); }
	void Sound::Pause() { _sound.pause(); }
	void Sound::Stop() { _sound.stop(); }

	bool Sound::IsLooping() const { return _sound.getLoop(); }
	float Sound::GetPitch() const { return _sound.getPitch(); }

	void Sound::SetLooping(const bool& looping) { _sound.setLoop(looping); }
	void Sound::SetPitch(const float& pitch) { _sound.setPitch(pitch); }
}
