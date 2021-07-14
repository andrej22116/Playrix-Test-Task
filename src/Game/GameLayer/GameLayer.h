#pragma once
#include <ViewLayer/ViewLayer.h>
#include <Game/GameObjects/Gun/Gun.h>
#include <Game/GameObjects/Crosshair/Crosshair.h>
#include <Game/GameObjects/Bullet/Bullet.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class AimObject;

class GameLayer : public ViewLayer
{
public:
	GameLayer();

	//virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept override;

	virtual void onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept override;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept override;
	virtual void onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept override;

	virtual ~GameLayer() override;

private:
	void collisionBroadPhase();
	void collisionNarrowPhase();

	void testHit(const sf::Vector2f& point);
	void registerAim(AimObject* aimObject);

private:
	Gun _gun;
	Crosshair _crosshair;
	std::unordered_set<Bullet*> _bulletSet;
	std::vector<Bullet*> _bulletForDeletePretendents;
	std::unordered_map<AimObject*, std::unordered_set<AimObject*>> _aimCollisionMap;
	std::vector<AimObject*> _aimList;
	Bullet::CallbackType _bulletCallback;
};

