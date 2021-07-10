#include "Gun.h"
#include <Game/GameSources/GameSources.h>

Gun::Gun(ViewLayer* owner)
	: GraphicsObject(owner)
	, _rect(-25, -100, 50, 100)
	, _ready(true)
{
	_gun.setSize({ _rect.width, _rect.height });
	_gun.setTexture(&GameSources::texture("gun_texture"));
}

sf::Rect<float> Gun::rect() const noexcept
{
	return _rect;
}

void Gun::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	_gun.setPosition(calcDrawPosition());
	renderTarget.draw(_gun);
}

void Gun::update(double updateFrequency, double timeDeviation) noexcept
{
}

bool Gun::ready() const noexcept
{
	return false;
}

sf::Vector2f Gun::calcDrawPosition() const
{
	return {
		x() + _rect.left,
		y() + _rect.top
	};
}
