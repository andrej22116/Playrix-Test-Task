#include "GameSources.h"
#include <fstream>

using json = nlohmann::json;

GameSources::GameSources()
{
	_defaultConfig = R"(
		{
			"sources": {
				"main_font": {
					"type": "font",
					"value": "./src/fonts/13888.otf"
				},
				"info_font": {
					"type": "font",
					"value": "./src/fonts/20686.ttf"
				},
				"bg_texture": {
					"type": "texture",
					"value": "./src/img/bg.jpg"
				},
				"gun_texture": {
					"type": "texture",
					"value": "./src/img/gun.png"
				},
				"bullet_texture": {
					"type": "texture",
					"value": "./src/img/bullet.png"
				},
				"crosshair_texture": {
					"type": "texture",
					"value": "./src/img/crosshair.png"
				},
				"calm_aim_texture": {
					"type": "texture",
					"value": "./src/img/calm_aim.png"
				},
				"fast_aim_texture": {
					"type": "texture",
					"value": "./src/img/fast_aim.png"
				}
			},
			"game_config": {
			}
		}
	)"_json;
}

GameSources& GameSources::instance()
{
	static GameSources instance;
	return instance;
}

void GameSources::initSources(const std::string& configPath)
{
	json config;
	std::ifstream ifstream{ configPath };
	if (ifstream) {
		throw std::invalid_argument("File \"" + configPath + "\" not exists!");
	}

	ifstream >> config;
	ifstream.close();

	initSources(config);
}

void GameSources::initSources(const json& config)
{
	instance().m_initSources(config);
}

void GameSources::initSources()
{
	initSources(instance()._defaultConfig);
}

sf::Font& GameSources::font(const std::string& name)
{
	auto& instance = GameSources::instance();
	auto iterator = instance._fontMap.find(name);
	if ( iterator == instance._fontMap.end() ) {
		throw std::invalid_argument("Font with name \"" + name + "\" not found! Check config.");
	}

	return iterator->second;
}

sf::Texture& GameSources::texture(const std::string& name)
{
	auto& instance = GameSources::instance();
	auto iterator = instance._textureMap.find(name);
	if ( iterator == instance._textureMap.end() ) {
		throw std::invalid_argument("Texture with name \"" + name + "\" not found! Check config.");
	}

	return iterator->second;
}

const json& GameSources::defaultConfig() noexcept
{
	return instance()._defaultConfig;
}

void GameSources::m_initSources(const json& config)
{
	if (!config.contains("sources")) {
		throw std::invalid_argument("Config not contains \"sources\"!");
	}

	std::unordered_map < std::string, std::function<void(const std::string&, const std::string&)>> appendByTypeFuncMap{
		{"texture", [this](const std::string& name, const std::string& path) {
			const auto& [iter, _] = _textureMap.emplace(std::make_pair(name, sf::Texture{}));
			auto& [__, texture] = *iter;
			texture.loadFromFile(path);
			texture.setSmooth(true);
		}},
		{"font", [this](const std::string& name, const std::string& path) {
			const auto& [iter, _] = _fontMap.emplace(std::make_pair(name, sf::Font{}));
			auto& [__, font] = *iter;
			font.loadFromFile(path);
		}},
	};

	for ( const auto& [key, value] : config["sources"].items() ) {
		if ( !value.contains("type") || !value.contains("value")) {
			continue;
		}
		auto appendFunctionIterator = appendByTypeFuncMap.find(value["type"].get<std::string>());
		if ( appendFunctionIterator != appendByTypeFuncMap.end() ) {
			auto& [_, func] = *appendFunctionIterator;
			func(key, value["value"].get<std::string>());
		}
	}
}
