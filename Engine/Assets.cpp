#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "Assets.h"

namespace maoutch
{
	Assets* Assets::_instance = nullptr;
	std::mutex Assets::_mutex;

	Assets::Assets() = default;
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
		if (!Instance()->_LoadSounds()) return false;
		
		return true;
	}

	nlohmann::json& Assets::Config() { return Instance()->_jsonData; }

	std::map<std::string, sf::Texture>& Assets::GetTexturesMap() { return _textures; }

	sf::Texture& Assets::GetTexture(const std::string& name) { return _textures.at(name); }
	sf::Font& Assets::GetFont() { return _font; }
	sf::SoundBuffer& Assets::GetSoundBuffer(const std::string& name) { return _sounds.at(name); }

	bool Assets::_LoadTexture(const std::string& name, const std::string& fileName)
	{
		const bool loaded = _textures[name].loadFromFile(Config<std::string>("Assets", "TexturePath") + fileName);
		return loaded;
	}
	bool Assets::_LoadSound(const std::string& name, const std::string& fileName)
	{
		const bool loaded = _sounds[name].loadFromFile(Config<std::string>("Assets", "SoundPath") + fileName);
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
		if (!_LoadTexture("Dragon", "boss_dragon.png")) return false;
		if (!_LoadTexture("Ent", "creeper.png")) return false;
		if (!_LoadTexture("Elemental", "elemental_wisp.png")) return false;
		if (!_LoadTexture("Imp", "imp.png")) return false;
		if (!_LoadTexture("Plant", "man_eating_plant.png")) return false;
		if (!_LoadTexture("Mimic", "mimic.png")) return false;
		if (!_LoadTexture("Ghost", "shadow.png")) return false;
		if (!_LoadTexture("Slime", "slime.png")) return false;

		if (!_LoadTexture("BackGround", "test_background.png")) return false;

		return true;
	}
	bool Assets::_LoadFonts()
	{
		const auto fontPath = Config<std::string>("Assets", "FontPath");

		if (!_font.loadFromFile(fontPath + "minecraftia.ttf")) return false;

		return true;
	}
	bool Assets::_LoadSounds()
	{
		if (!_LoadSound("Bullet Shoot", "bullet_shoot.wav")) return false;
		if (!_LoadSound("Explosion", "explosion.wav")) return false;

		return true;
	}
}