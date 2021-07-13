#pragma once
#include <GraphicsObject/GraphicsObject.h>
#include <functional>

class AimObject : public GraphicsObject
{
public:
	AimObject(float radius);

	void setMovingArea(const sf::Rect<float>& movingArea);
	void setMoveDirection(const sf::Vector2f& direction);
	const sf::Vector2f& moveDirection() const;
	void move(float delta);
	bool hitTest(const sf::Vector2f& hitPosition);

	virtual void update(double updateFrequency, double timeDeviation) noexcept;

public:
	virtual bool registerHit() = 0;

	virtual void afterResolveCollision();

public:
	static void resolveCollision(AimObject* first, AimObject* second);

private:
	sf::Rect<float> _movingArea;
	sf::Vector2f _moveDirection;
	float _radius;
	float _quadRadius;
};

