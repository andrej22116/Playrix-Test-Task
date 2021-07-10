#pragma once

#include <GraphicsObject/GraphicsObject.h>
#include <functional>

class Bullet : public GraphicsObject
{
public:
	Bullet(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float lifeTime);

	virtual sf::Rect<float> rect() const noexcept override;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept override;
	virtual void update(double updateFrequency, double timeDeviation) noexcept override;

	using CallbackType = std::function<void(Bullet*, const sf::Vector2f&)>;
	void setEndPointEventHandler(CallbackType callback);

private:
	sf::Vector2f _currentPoint;
	sf::Vector2f _endPoint;
	sf::Vector2f _vector;
	sf::Rect<float> _rect;
	sf::RectangleShape _bullet;
	CallbackType _eventHandler;
	float _depth;
	float _rotationSpeed;
	float _speed;
	float _lifeTime;
	float _maxLifeTime;
};

