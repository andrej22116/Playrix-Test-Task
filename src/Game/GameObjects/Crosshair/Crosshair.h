#pragma once

#include <GraphicsObject/GraphicsObject.h>

class Crosshair : public GraphicsObject
{
public:
	Crosshair();

	virtual sf::Rect<float> rect() const noexcept override;
	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept override;

private:
	sf::RectangleShape _crosshair;
	sf::Rect<float> _rect;
};

