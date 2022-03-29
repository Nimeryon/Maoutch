#pragma once
#include <map>
#include <mutex>
#include <string>

namespace sf
{
	class Texture;
	class Font;
}

namespace maoutch
{
	const std::string texturesPath = "Assets\\Textures\\";
	const std::string fontsPath = "Assets\\Fonts\\";

	class AssetLoader
	{
	public:
		static AssetLoader* GetInstance();

		AssetLoader(AssetLoader const&) = delete;
		void operator=(AssetLoader const&) = delete;

		bool Init();

		// Only texture file name needed not the full path
		bool LoadTexture(const std::string& name, const std::string& fileName);
		// Only font file name needed not the full path
		bool LoadFont(const std::string& name, const std::string& fileName);

		std::map<std::string, sf::Texture>& GetTexturesMap();
		sf::Texture& GetTexture(const std::string& name);

		std::map<std::string, sf::Font>& GetFontsMap();
		sf::Font& GetFont(const std::string& name);
		
	protected:
		AssetLoader();
		~AssetLoader();

	private:
		static AssetLoader* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::Font> _fonts;
	};
}