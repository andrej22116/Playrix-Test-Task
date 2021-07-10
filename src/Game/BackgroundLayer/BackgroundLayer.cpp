#include "BackgroundLayer.h"
#include <Game/GameSources/GameSources.h>

BackgroundLayer::BackgroundLayer()
{
	_backgroundRect.setPosition(0, 0);
	auto& texture = GameSources::texture("bg_texture");
	texture.setRepeated(true);
	texture.setSmooth(true);
	_backgroundRect.setTexture(texture);
}

void BackgroundLayer::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	renderTarget.draw(_backgroundRect);
}

void BackgroundLayer::onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept
{
	ViewLayer::onSizeEvent(sizeEvent);

	_backgroundRect.setTextureRect({ 0,0, static_cast<int>(sizeEvent.width), static_cast<int>(sizeEvent.height) });
}
