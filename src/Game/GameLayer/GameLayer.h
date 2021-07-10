#pragma once
#include <ViewLayer/ViewLayer.h>
#include <Game/GameObjects/Gun/Gun.h>
#include <Game/GameObjects/Crosshair/Crosshair.h>
#include <Game/GameObjects/Bullet/Bullet.h>
#include <unordered_set>
#include <vector>


class GameLayer : public ViewLayer
{
public:
	GameLayer();

	//virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept;

	virtual void onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept override;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept;
	virtual void onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept;

	virtual ~GameLayer() override;

private:

private:
	Gun _gun;
	Crosshair _crosshair;
	std::unordered_set<Bullet*> _bulletSet;
	std::vector<Bullet*> _deletePretendents;
	Bullet::CallbackType _bulletCallback;
};

