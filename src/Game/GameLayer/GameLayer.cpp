#include "GameLayer.h"
#include <Game/GameObjects/CalmAim/CalmAim.h>

GameLayer::GameLayer()
{
	_gun.setInViewLayer(this);
	_crosshair.setInViewLayer(this);

	enableConstUpdates(true);
	setListenMouseEvents(true);

	_bulletCallback = [this](Bullet* bullet, const sf::Vector2f& endPoint) {
		_bulletSet.erase(bullet);
		_bulletForDeletePretendents.push_back(bullet);
		testHit(endPoint);
	};

	auto size = this->size();

	auto calmAim = new CalmAim();
	calmAim->setMovingArea({ 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) });
	calmAim->setInViewLayer(this);
	calmAim->setPosition({ 100, 100 });

	registerAim(calmAim);

	calmAim = new CalmAim();
	calmAim->setMovingArea({ 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) });
	calmAim->setInViewLayer(this);
	calmAim->setPosition({ 500, 95 });

	registerAim(calmAim);
}

void GameLayer::update(double updateFrequency, double timeDeviation) noexcept
{
	ViewLayer::update(updateFrequency, timeDeviation);
	for ( auto bullet : _bulletForDeletePretendents) {
		bullet->removeFromViewLayer();
		delete bullet;
	}
	_bulletForDeletePretendents.clear();

	collisionBroadPhase();
	collisionNarrowPhase();
}

void GameLayer::onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept
{
	_gun.setPosition({ static_cast<float>(sizeEvent.width) * 0.5f, static_cast<float>(sizeEvent.height) });
	for ( auto& [aimPtr, _] : _aimCollisionMap ) {
		aimPtr->setMovingArea({ 0, 0, static_cast<float>(sizeEvent.width), static_cast<float>(sizeEvent.height) });
	}
}

bool GameLayer::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept
{
	sf::Vector2f startPos{ _gun.x(), _gun.y() - 45 };

	auto bullet = new Bullet(startPos, _crosshair.position(), 1);
	bullet->setInViewLayer(this);
	bullet->setEndPointEventHandler(_bulletCallback);
	_bulletSet.insert(bullet);

	return false;
}

void GameLayer::onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept
{
	_crosshair.setPosition({ static_cast<float>(mouseMoveEvent.x), static_cast<float>(mouseMoveEvent.y) });
	_gun.setX(mouseMoveEvent.x);
}

GameLayer::~GameLayer()
{
	for (auto bullet : _bulletSet) {
		bullet->removeFromViewLayer();
		delete bullet;
	}
	for (auto aim : _aimList) {
		aim->removeFromViewLayer();
		delete aim;
	}
}

void GameLayer::collisionBroadPhase()
{
	for (auto& [aim, closeAimSet] : _aimCollisionMap) {
		closeAimSet.clear();

		auto rect = aim->rect();
		rect.left += aim->x() - rect.width;
		rect.top += aim->y() - rect.height;
		rect.width *= 3;
		rect.height *= 3;

		auto closeObjects = selectGraphicsObjects(rect);
		closeAimSet.reserve(closeObjects.size());

		for ( auto objPtr : closeObjects ) {
			if (objPtr->name() == "aim") {
				closeAimSet.insert(static_cast<AimObject*>(objPtr));
			}
		}

		closeAimSet.erase(aim);
	}

	if ( _aimCollisionMap.size() < 2 ) {
		return;
	}

	auto iterator = _aimCollisionMap.begin();
	auto end = _aimCollisionMap.end();
	auto beforeEnd = end;
	--beforeEnd;

	for (; iterator != beforeEnd; ++iterator) {
		auto next = iterator;
		++next;

		for (; next != end; ++next) {
			next->second.erase(iterator->first);
		}
	}
}

void GameLayer::collisionNarrowPhase()
{
	for ( auto& [aim, closeAimsSet] : _aimCollisionMap ) {
		for ( auto closeAim : closeAimsSet ) {
			AimObject::resolveCollision(aim, closeAim);
		}
	}
}

void GameLayer::testHit(const sf::Vector2f& point)
{
	auto hitObjects = selectGraphicsObjects({ point.x, point.y, point.x + 1, point.y + 1 });
	for ( auto objectPtr : hitObjects ) {
		if (objectPtr->name() == "aim") {
			auto aim = static_cast<AimObject*>(objectPtr);
			if ( aim->hitTest(point) && aim->registerHit() ) {
				_aimCollisionMap.erase(aim);
			}
		}
	}
}

void GameLayer::registerAim(AimObject* aimObject)
{
	_aimCollisionMap[aimObject] = {};
	_aimList.push_back(aimObject);
}
