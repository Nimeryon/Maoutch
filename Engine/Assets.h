#pragma once
#include <map>
#include <mutex>
#include <string>
#include <json.hpp>

namespace sf
{
	class Texture;
	class Font;
}

namespace maoutch
{
	const std::string texturesPath = "Assets\\Textures\\";
	const std::string fontsPath = "Assets\\Fonts\\";

	class Assets
	{
	public:
		static Assets* GetInstance();

		Assets(Assets const&) = delete;
		void operator=(Assets const&) = delete;

		static bool Init();
		static nlohmann::json& Config();

		// Only texture file name needed not the full path
		bool LoadTexture(const std::string& name, const std::string& fileName);
		// Only font file name needed not the full path
		bool LoadFont(const std::string& name, const std::string& fileName);

		std::map<std::string, sf::Texture>& GetTexturesMap();
		sf::Texture& GetTexture(const std::string& name);

		std::map<std::string, sf::Font>& GetFontsMap();
		sf::Font& GetFont(const std::string& name);
		
	protected:
		Assets();
		~Assets();

	private:
		static Assets* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::Font> _fonts;

		nlohmann::json _jsonData;
	};
}