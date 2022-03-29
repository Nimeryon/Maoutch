#include <SFML/Graphics.hpp>

#include "AssetLoader.h"

namespace maoutch
{
	AssetLoader* AssetLoader::_instance = nullptr;
	std::mutex AssetLoader::_mutex;

	AssetLoader::AssetLoader() = default;
	AssetLoader::~AssetLoader()
	{
		_textures.clear();
		_fonts.clear();
	}

	AssetLoader* AssetLoader::GetInstance()
	{
		std::lock_guard lock(_mutex);

		if (_instance == nullptr) _instance = new AssetLoader();
		return _instance;
	}

	bool AssetLoader::Init()
	{
		if (!LoadTexture("Effects", "effect.png")) return false;
		if (!LoadTexture("Elements", "elements.png")) return false;

		return true;
	}

	bool AssetLoader::LoadTexture(const std::string& name, const std::string& fileName)
	{
		sf::Texture texture;
		if (texture.loadFromFile(texturesPath + fileName))
		{
			_textures[name] = texture;
			return true;
		}
		return false;
	}
	bool AssetLoader::LoadFont(const std::string& name, const std::string& fileName)
	{
		sf::Font font;
		if (font.loadFromFile(fontsPath + fileName))
		{
			_fonts[name] = font;
			return true;
		}
		return false;
	}

	std::map<std::string, sf::Texture>& AssetLoader::GetTexturesMap() { return _textures; }
	sf::Texture& AssetLoader::GetTexture(const std::string& name) { return _textures.at(name); }

	std::map<std::string, sf::Font>& AssetLoader::GetFontsMap() { return _fonts; }
	sf::Font& AssetLoader::GetFont(const std::string& name) { return _fonts.at(name); }
}