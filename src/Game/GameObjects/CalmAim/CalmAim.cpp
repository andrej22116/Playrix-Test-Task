#include "CalmAim.h"
#include <Game/GameSources/GameSources.h>

CalmAim::CalmAim()
	: AimObject(50)
	, _rect(-50, -50, 100, 100)
	, _speed(2)
	, _hp(5)
{
	_shape.setTexture(&GameSources::texture("calm_aim_texture"));
	_shape.setTextureRect({ 200, 0, 200, 200 });
	_shape.setSize({ 100, 100 });
	
	setMoveDirection(sf::Vector2f{ 1, 0 } * _speed);
}

sf::Rect<float> CalmAim::rect() const noexcept
{
	return _rect;
}

void CalmAim::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	renderTarget.draw(_shape);
}

void CalmAim::update(double updateFrequency, double timeDeviation) noexcept
{
	move((updateFrequency + timeDeviation) / updateFrequency);

	auto pos = this->position();
	pos.x += _rect.left;
	pos.y += _rect.top;
	_shape.setPosition(pos);

	AimObject::update(updateFrequency, timeDeviation);
}

bool CalmAim::registerHit()
{
	--_hp;
	return _hp == 0;
}

void CalmAim::afterResolveCollision()
{
	auto& dir = moveDirection();
	setMoveDirection(dir.x >= 0 ? sf::Vector2f{ 1, 0 } *_speed : sf::Vector2f{ -1, 0 } *_speed);
}

