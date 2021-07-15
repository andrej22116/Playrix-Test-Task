#include "GameLayer.h"
#include <Game/GameObjects/CalmAim/CalmAim.h>
#include <Game/GameObjects/FastAim/FastAim.h>
#include <Utils/Utils.hpp>
#include <random>

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

	ViewLayer::onSizeEvent(sizeEvent);
}

bool GameLayer::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept
{
	sf::Vector2f startPos{ _gun.x(), _gun.y() - 45 };

	auto bullet = new Bullet(startPos, _crosshair.position(), 0.5);
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

void GameLayer::initGame()
{
	deleteObjects();

	_crosshair.removeFromViewLayer();
	_gun.removeFromViewLayer();

	auto& size = this->size();
	std::random_device rd;
	std::uniform_real_distribution<float> dist(-1, 1);
	std::uniform_real_distribution<float> posDist(100, (size.width > size.height ? size.width : size.height) - 100);

	for ( int i = 0; i < 10; ++i ) {
		AimObject* aim = nullptr;
		if (dist(rd) < 0) {
			aim = new CalmAim();
		}
		else {
			aim = new FastAim(utils::normalize(sf::Vector2f{ dist(rd), dist(rd) }));
		}

		aim->setPosition({ posDist(rd), posDist(rd) });
		aim->setMovingArea({ 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) });
		aim->setInViewLayer(this);

		registerAim(aim);
	}

	_crosshair.setInViewLayer(this);
	_gun.setInViewLayer(this);
}

void GameLayer::setEndGameHandler(std::function<void()> callback)
{
	_onEndGameHandler = callback;
}

GameLayer::~GameLayer()
{
	deleteObjects();
}

void GameLayer::deleteObjects()
{
	for (auto bullet : _bulletSet) {
		bullet->removeFromViewLayer();
		delete bullet;
	}
	_bulletSet.clear();
	for (auto aim : _aimList) {
		aim->removeFromViewLayer();
		delete aim;
	}
	_aimList.clear();
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
			if ( objPtr->name() == "aim" 
				&& _aimCollisionMap.find(static_cast<AimObject*>(objPtr)) != _aimCollisionMap.end() ) {
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
				if (_aimCollisionMap.empty() && _onEndGameHandler != nullptr) {
					_onEndGameHandler();
				}
			}
		}
	}
}

void GameLayer::registerAim(AimObject* aimObject)
{
	_aimCollisionMap[aimObject] = {};
	_aimList.push_back(aimObject);
}
