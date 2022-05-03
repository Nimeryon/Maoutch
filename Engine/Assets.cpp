#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "Assets.h"

namespace maoutch
{
	Assets* Assets::_instance = nullptr;
	std::mutex Assets::_mutex;

	Assets::Assets() : _isSmoothFont(false) {}
	Assets::~Assets() = default;

	Assets* Assets::Instance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new Assets();
		return _instance;
	}
	
	bool Assets::Init()
	{
		std::ifstream file("Assets\\config.json");
		file >> Instance()->_jsonData;

		if (!Instance()->_LoadTextures()) return false;
		if (!Instance()->_LoadFonts()) return false;
		
		return true;
	}

	nlohmann::json& Assets::Config() { return Instance()->_jsonData; }

	std::map<std::string, sf::Texture>& Assets::GetTexturesMap() { return _textures; }
	sf::Texture& Assets::GetTexture(const std::string& name) { return _textures.at(name); }

	void Assets::SetSmoothFont(const bool& isSmooth)
	{
		_isSmoothFont = isSmooth;
		onFontChange(GetFont());
	}

	sf::Font& Assets::GetFont() { return _isSmoothFont ? _robotoFont : _minecraftiaFont; }

	bool Assets::_LoadTexture(const std::string& name, const std::string& fileName)
	{
		const bool loaded = _textures[name].loadFromFile(Config<std::string>("Assets", "TexturePath") + fileName);

		return loaded;
	}

	bool Assets::_LoadTextures()
	{
		if (!_LoadTexture("Elements", "elements.png")) return false;
		if (!_LoadTexture("Elements Background", "elements_background.png")) return false;
		if (!_LoadTexture("Selected Animation", "selected_animation.png")) return false;
		if (!_LoadTexture("Hint Arrow Animation", "hint_arrow_animation.png")) return false;
		if (!_LoadTexture("HealthBar", "healthBar.png")) return false;
		if (!_LoadTexture("HealthBar Eye", "healthBarEye.png")) return false;
		if (!_LoadTexture("HealthBar Skull", "healthBarSkull.png")) return false;
		if (!_LoadTexture("Skull Decorators", "skull_decorators.png")) return false;
		
		if (!_LoadTexture("Demon", "demon.png")) return false;
		if (!_LoadTexture("Cerbere", "cerbere.png")) return false;

		if (!_LoadTexture("BackGround", "test_background.png")) return false;

		return true;
	}
	bool Assets::_LoadFonts()
	{
		const auto fontPath = Config<std::string>("Assets", "FontPath");

		if (!_minecraftiaFont.loadFromFile(fontPath + "minecraftia.ttf")) return false;
		if (!_robotoFont.loadFromFile(fontPath + "roboto.ttf")) return false;

		return true;
	}
}