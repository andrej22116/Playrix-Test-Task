#include "Bullet.h"
#include <Game/GameSources/GameSources.h>
#include <Utils/Utils.hpp>

Bullet::Bullet(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float lifeTime)
	: _currentPoint(startPoint)
	, _endPoint(endPoint)
	, _vector(utils::normalize(endPoint - startPoint))
	, _rect(-15, -15, 30, 30)
	, _rotationSpeed(12)
	, _lifeTime(0)
	, _maxLifeTime(lifeTime)
{
	setName("bullet");

	_bullet.setTexture(&GameSources::texture("bullet_texture"));
	_bullet.setSize({ _rect.width, _rect.height });

	_bullet.setOrigin({ static_cast<float>(_rect.width) * 0.5f, static_cast<float>(_rect.height) * 0.5f });

	_speed = utils::length(endPoint - startPoint) / _maxLifeTime;
}

sf::Rect<float> Bullet::rect() const noexcept
{
	return sf::Rect<float>();
}

void Bullet::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	auto position = this->position();
	_bullet.setPosition({ position });
	renderTarget.draw(_bullet);
}

void Bullet::update(double updateFrequency, double timeDeviation) noexcept
{
	_lifeTime += updateFrequency + timeDeviation;

	float scaleFactor = (_lifeTime / _maxLifeTime);
	float scale = std::log(-scaleFactor + 1) * 0.2  + 1;
	_bullet.setScale({ scale, scale });
	_bullet.setRotation(_bullet.getRotation() + _rotationSpeed);

	auto offset = _speed * (timeDeviation + updateFrequency);
	auto offsetVector = _vector;
	offsetVector.x *= offset;
	offsetVector.y *= offset; 

	_currentPoint += offsetVector;

	auto newDirectionVector = utils::normalize(_endPoint - _currentPoint);
	this->setPosition(_currentPoint);

	if ( !utils::isVectorsEqual(_vector, newDirectionVector ) && _eventHandler != nullptr ) {
		_eventHandler(this, _endPoint);
	}
}

void Bullet::setEndPointEventHandler(CallbackType callback)
{
	_eventHandler = callback;
}
