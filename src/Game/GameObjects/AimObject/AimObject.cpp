#include "AimObject.h"
#include <Utils/Utils.hpp>

AimObject::AimObject(float radius)
	: _radius(radius)
	, _quadRadius(radius * radius)
{
	setName("aim");
}

void AimObject::setMovingArea(const sf::Rect<float>& movingArea)
{
	_movingArea.top = movingArea.top + _radius;
	_movingArea.left = movingArea.left + _radius;
	_movingArea.width = movingArea.width - _radius * 2;
	_movingArea.height = movingArea.height - _radius * 2;
}

const sf::Rect<float>& AimObject::movingArea() const
{
	return _movingArea;
}

void AimObject::setMoveDirection(const sf::Vector2f& direction)
{
	_moveDirection = direction;
}

const sf::Vector2f& AimObject::moveDirection() const
{
	return _moveDirection;
}

void AimObject::move(float delta)
{
	auto pos = this->position();
	pos += _moveDirection * delta;
	this->setPosition(pos);
}

bool AimObject::hitTest(const sf::Vector2f& hitPosition)
{
	auto difX = hitPosition.x - this->x();
	auto difY = hitPosition.y - this->y();

	return (difX * difX + difY * difY) <= _quadRadius;
}

void AimObject::update(double updateFrequency, double timeDeviation) noexcept
{
	auto position = this->position();
	if (position.x <= _movingArea.left || position.x >= _movingArea.left + _movingArea.width) {
		_moveDirection.x = -_moveDirection.x;
	}
	if (position.y <= _movingArea.top || position.y >= _movingArea.top + _movingArea.height) {
		_moveDirection.y = -_moveDirection.y;
	}
}

void AimObject::afterResolveCollision()
{
}

void AimObject::resolveCollision(AimObject* first, AimObject* second)
{
	if ( !first || !second ) {
		return;
	}

	auto firstPos = first->position();
	auto secondPos = second->position();

	auto diffX = firstPos.x - secondPos.x;
	auto diffY = firstPos.y - secondPos.y;
	auto radius = first->_radius + second->_radius;

	auto distance = std::sqrt(diffX * diffX + diffY * diffY) - radius;
	if (distance >= 0) {
		return;
	}

	/*sf::Vector2f fixVector{distance * 0.5f, distance * 0.5f};
	auto fixedFirstMoveVector = first->_moveDirection - fixVector;
	auto fixedSecondMoveVector = second->_moveDirection - fixVector;

	firstPos -= fixedFirstMoveVector;
	secondPos -= fixedSecondMoveVector;*/

	first->setPosition(firstPos);
	second->setPosition(secondPos);

	auto n = utils::normalize(firstPos - secondPos);
	first->_moveDirection -= (n * utils::dot(n, first->_moveDirection)) * 2.0f;

	n *= -1.0f;
	second->_moveDirection -= (n * utils::dot(n, second->_moveDirection)) * 2.0f;

	first->afterResolveCollision();
	second->afterResolveCollision();
}
