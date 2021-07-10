#include "Crosshair.h"
#include <Game/GameSources/GameSources.h>

Crosshair::Crosshair()
    : _rect({ -20, -20, 40, 40 })
{
    _crosshair.setSize({ _rect.width, _rect.height });
    _crosshair.setTexture(&GameSources::texture("crosshair_texture"));
}

sf::Rect<float> Crosshair::rect() const noexcept
{
    return _rect;
}

void Crosshair::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
    auto position = Crosshair::position();
    _crosshair.setPosition({ position.x + _rect.left, position.y + _rect.top });
    renderTarget.draw(_crosshair);
}
