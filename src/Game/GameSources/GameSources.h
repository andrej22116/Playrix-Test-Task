#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <json.h>

using json = nlohmann::json;

class GameSources
{
	GameSources();

public:
	GameSources(const GameSources&) = delete;
	GameSources(GameSources&&) = delete;
	GameSources& operator = (const GameSources&) = delete;
	GameSources& operator = (GameSources&&) = delete;

	static GameSources& instance();

	static void initSources(const std::string& configPath);
	static void initSources(const json& config);
	static void initSources();

	static sf::Font& font(const std::string& name);
	static sf::Texture& texture(const std::string& name);

	static const json& defaultConfig() noexcept;

private:
	void m_initSources(const json& config);

private:
	json _defaultConfig;
	std::string _backgroundTexturePath;

	std::unordered_map<std::string, sf::Texture> _textureMap;
	std::unordered_map<std::string, sf::Font> _fontMap;
};
