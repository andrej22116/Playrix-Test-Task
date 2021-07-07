#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer()
{
	_backgroundRect.setPosition(0, 0);
	_backgroundTile.loadFromFile("./src/img/bg.jpg");
	_backgroundTile.setRepeated(true);
	_backgroundTile.setSmooth(true);
	_backgroundRect.setTexture(_backgroundTile);
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
