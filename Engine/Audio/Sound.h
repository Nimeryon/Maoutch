#pragma once
namespace sf
{
	class SoundBuffer;
}
#include <SFML/Audio/Sound.hpp>

namespace maoutch
{
	class Sound
	{
	public:
		explicit Sound(const sf::SoundBuffer& buffer, const float& pitch = 1);
		virtual ~Sound();

		void Play();
		void Pause();
		void Stop();

		// Getters

		bool IsLooping() const;
		float GetPitch() const;

		// Setters

		void SetLooping(const bool& looping);
		void SetPitch(const float& pitch);

	private:
		sf::Sound _sound;
	};
}
