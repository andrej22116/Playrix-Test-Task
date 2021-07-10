#pragma once
#include <ViewLayer/ViewLayer.h>
#include <SFML/Graphics.hpp>

class BackgroundLayer : public ViewLayer
{
public:
	BackgroundLayer();

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;

	virtual void onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept override;

private:
	sf::RenderTexture _backgroundTexture;
	sf::Sprite _backgroundRect;
};

