#include <SFML/Graphics.hpp>
#include <fstream>

#include "Assets.h"

#include <iostream>

namespace maoutch
{
	Assets* Assets::_instance = nullptr;
	std::mutex Assets::_mutex;

	Assets::Assets() = default;
	Assets::~Assets() = default;

	Assets* Assets::GetInstance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Assets();
		return _instance;
	}
	
	bool Assets::Init()
	{
		std::ifstream file("Assets\\config.json");
		file >> GetInstance()->_jsonData;

		if (!GetInstance()->LoadTexture("Elements", "elements.png")) return false;
		if (!GetInstance()->LoadTexture("Elements Background", "elements_background.png")) return false;
		if (!GetInstance()->LoadTexture("Selected Animation", "selected_animation.png")) return false;
		if (!GetInstance()->LoadTexture("Hint Arrow Animation", "hint_arrow_animation.png")) return false;

		return true;
	}

	nlohmann::json& Assets::Config() { return GetInstance()->_jsonData; }

	bool Assets::LoadTexture(const std::string& name, const std::string& fileName)
	{
		if (_textures[name].loadFromFile(Config<std::string>("Assets", "TexturePath") + fileName)) return true;
		return false;
	}
	bool Assets::LoadFont(const std::string& name, const std::string& fileName)
	{
		if (_fonts[name].loadFromFile(Config<std::string>("Assets", "FontPath") + fileName)) return true;
		return false;
	}
	
	std::map<std::string, sf::Texture>& Assets::GetTexturesMap() { return _textures; }
	sf::Texture& Assets::GetTexture(const std::string& name) { return _textures.at(name); }
	
	std::map<std::string, sf::Font>& Assets::GetFontsMap() { return _fonts; }
	sf::Font& Assets::GetFont(const std::string& name) { return _fonts.at(name); }
}
