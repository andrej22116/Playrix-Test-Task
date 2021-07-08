#include "GameSources.h"

GameSources& GameSources::instance()
{
	static GameSources instance;
	return instance;
}

void GameSources::initSources(const std::string& configPath)
{
	auto& instance = GameSources::instance();

	instance._mainFont.loadFromFile("./src/fonts/13888.otf");
	instance._backgroundTexturePath = "./src/img/bg.jpg";
}

const sf::Font& GameSources::mainFont()
{
	return instance()._mainFont;
}

const std::string& GameSources::backgroundTexturePath()
{
	return instance()._backgroundTexturePath;
}
