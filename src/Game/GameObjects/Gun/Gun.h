#pragma once
#include <GraphicsObject/GraphicsObject.h>

class Gun : public GraphicsObject
{
public:
	Gun(ViewLayer* owner = nullptr);

	virtual sf::Rect<float> rect() const noexcept override;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept override;
	virtual void update(double updateFrequency, double timeDeviation) noexcept override;

	bool ready() const noexcept;

private:
	sf::Vector2f calcDrawPosition() const;

private:
	sf::Rect<float> _rect;
	sf::RectangleShape _gun;
	sf::RectangleShape _bullet;

	bool _ready;

};

