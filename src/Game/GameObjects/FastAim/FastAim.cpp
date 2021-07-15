#include "FastAim.h"
#include <Game/GameSources/GameSources.h>

FastAim::FastAim(const sf::Vector2f& startDirection)
	: AimObject(25)
	, _rect(-30, -30, 60, 60)
	, _shape({ 60, 60 })
	, _speed(5)
	, _hp(1)
{
	_shape.setTexture(&GameSources::texture("fast_aim_texture"));
	_shape.setTextureRect({ 0, 0, 200, 200 });
	_shape.setSize({ 60, 60 });
	_shape.setOrigin(30, 30);

	setMoveDirection(startDirection * _speed);
}

sf::Rect<float> FastAim::rect() const noexcept
{
	return _rect;
}

void FastAim::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	renderTarget.draw(_shape);
}

void FastAim::update(double updateFrequency, double timeDeviation) noexcept
{
	auto deltaTime = (updateFrequency + timeDeviation) / updateFrequency;

	if (_hp <= 0) {
		dieMove(deltaTime * 0.5);
		auto& movingArea = this->movingArea();
		if (y() > movingArea.top + movingArea.height) {
			removeFromViewLayer();
		}
	}

	move(deltaTime);

	auto pos = this->position();
	_shape.setPosition(pos);

	if (_hp > 0) {
		AimObject::update(updateFrequency, timeDeviation);
	}

	moveDirection().x > 0 ? _shape.setScale(1, 1) : _shape.setScale(-1, 1);
	
}

bool FastAim::registerHit()
{
	_hp = 0;
	_shape.setTextureRect({ 200, 0, 200, 200 });
	return true;
}

void FastAim::dieMove(float fallingSpeed)
{
	sf::Vector2f fallingVector{ 0, fallingSpeed };

	setMoveDirection(moveDirection() + fallingVector);
}
