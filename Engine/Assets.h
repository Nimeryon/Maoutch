#pragma once
#include <map>
#include <mutex>
#include <string>
#include <json.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../Tools/Event.h"

namespace maoutch
{
	class Assets
	{
	public:
		static Assets* Instance();

		Assets(Assets const&) = delete;
		void operator=(Assets const&) = delete;

		static bool Init();

		static nlohmann::json& Config();
		template <typename T>
		static T Config(const std::string& category, const std::string& value)
		{
			return Config()[category][value].get<T>();
		}

		std::map<std::string, sf::Texture>& GetTexturesMap();

		sf::Texture& GetTexture(const std::string& name);
		sf::Font& GetFont();
		sf::SoundBuffer& GetSoundBuffer(const std::string& name);
		
	protected:
		Assets();
		~Assets();

	private:
		static Assets* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::SoundBuffer> _sounds;
		
		sf::Font _font;

		nlohmann::json _jsonData;
		
		bool _LoadTexture(const std::string& name, const std::string& fileName);
		bool _LoadSound(const std::string& name, const std::string& fileName);

		bool _LoadTextures();
		bool _LoadFonts();
		bool _LoadSounds();
	};
}