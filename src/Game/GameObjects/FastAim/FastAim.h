#pragma once

#include <Game/GameObjects/AimObject/AimObject.h>

class FastAim : public AimObject
{
public:
	FastAim(const sf::Vector2f& startDirection);

	virtual sf::Rect<float> rect() const noexcept override;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept;
	virtual bool registerHit() override;

private:
	void dieMove(float fallingSpeed);

private:
	sf::Rect<float> _rect;
	sf::RectangleShape _shape;
	float _speed;
	int _hp;
};

