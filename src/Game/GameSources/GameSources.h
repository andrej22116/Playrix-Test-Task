#pragma once

#include <SFML/Graphics.hpp>

class GameSources
{
	GameSources() = default;

public:
	GameSources(const GameSources&) = delete;
	GameSources(GameSources&&) = delete;
	GameSources& operator = (const GameSources&) = delete;
	GameSources& operator = (GameSources&&) = delete;

	static GameSources& instance();

	static void initSources(const std::string& configPath);

	static const sf::Font& mainFont();
	static const std::string& backgroundTexturePath();

private:
	sf::Font _mainFont;
	std::string _backgroundTexturePath;
};

