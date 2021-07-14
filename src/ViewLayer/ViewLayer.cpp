#include "ViewLayer.h"
#include <SFML/Graphics.hpp>
#include <GraphicsObject/GraphicsObject.h>

struct ViewLayer::GraphicsObjectData {
	GraphicsObject* graphicsObject;
	std::vector<sf::Vector2<float>> shape;
	sf::Rect<float> rect;
	bool acceptsMouseEvents;
};

ViewLayer::ViewLayer()
	: _notUsedId(1)
	, _aabbTree(2, 0, { true, true }, { 1,1 }, 2048)
	, _listenMouseEvents(false)
	, _needConstUpdates(false)
{
}

ViewLayer::ViewLayer(ViewLayer&& viewLayer) noexcept
	: _objectsMap(std::move(viewLayer._objectsMap))
	, _freeIdList(std::move(viewLayer._freeIdList))
	, _notUsedId(viewLayer._notUsedId)
	, _aabbTree(
		2,
		0,
		{ true, true }, 
		{ 
			static_cast<double>(viewLayer._layerSize.width),
			static_cast<double>(viewLayer._layerSize.height) 
		},
		2048
	)
	, _listenMouseEvents(false)
	, _needConstUpdates(false)
{
	for (auto& [id, _] : _objectsMap) {
		auto& aabb = viewLayer._aabbTree.getAABB(id);
		_aabbTree.insertParticle(id, aabb.lowerBound, aabb.upperBound);
	}
}

ViewLayer& ViewLayer::operator = (ViewLayer&& viewLayer) noexcept
{
	_objectsMap = std::move(viewLayer._objectsMap);
	_freeIdList = std::move(viewLayer._freeIdList);
	_notUsedId = viewLayer._notUsedId;

	return *this;
}

ObjectId ViewLayer::appendGraphicsObject(GraphicsObject* graphicsObject)
{
	if (!graphicsObject) {
		return 0;
	}

	graphicsObject->removeFromViewLayer();

	auto objectId = getId();

	auto [objectDataIter, _] = _objectsMap.emplace(std::make_pair( objectId, GraphicsObjectData{} ));
	auto& [__, objectData] = *objectDataIter;

	objectData.graphicsObject = graphicsObject;
	objectData.rect = graphicsObject->rect();
	objectData.shape = graphicsObject->shape();
	objectData.acceptsMouseEvents = graphicsObject->acceptsMouseEvents();

	auto objectPosition = graphicsObject->position();
	auto posX = objectPosition.x - objectData.rect.left;
	auto posY = objectPosition.y - objectData.rect.top;

	_aabbTree.insertParticle(
		objectId, 
		{ 
			posX,
			posY
		}, 
		{ 
			posX + objectData.rect.width,
			posY + objectData.rect.height
		}
	);

	_aabbTree.query({ {101,101}, {102, 102} });

	return objectId;
}

bool ViewLayer::removeGraphicsObject(GraphicsObject* graphicsObject)
{
	auto id = checkGraphicsObject(graphicsObject);
	if ( !id ) {
		return false;
	}

	if (_update) {
		_deferredDeletionList.push_back(id);
		return false;
	}

	_aabbTree.removeParticle(id);
	_objectsMap.erase(id);

	return true;
}

void ViewLayer::updateGraphicsObjectGeometry(GraphicsObject* graphicsObject)
{
	auto id = checkGraphicsObject(graphicsObject);
	if ( !id ) {
		return;
	}

	auto& graphicsObjectData = _objectsMap[id];
	auto objectPosition = graphicsObjectData.graphicsObject->position();
	graphicsObjectData.rect = graphicsObjectData.graphicsObject->rect();
	graphicsObjectData.shape = graphicsObjectData.graphicsObject->shape();

	updateGraphicsObjectAabb(id, graphicsObjectData);
}

void ViewLayer::updateGraphicsObjectPosition(GraphicsObject* graphicsObject)
{
	auto id = checkGraphicsObject(graphicsObject);
	if (!id) {
		return;
	}

	auto& graphicsObjectData = _objectsMap[id];
	updateGraphicsObjectAabb(id, graphicsObjectData);
}

void ViewLayer::enableSendingMouseEventsToGraphicsObject(GraphicsObject* graphicsObject, bool enabled)
{
	auto id = checkGraphicsObject(graphicsObject);
	if ( !id ) {
		return;
	}

	auto& graphicsObjectData = _objectsMap[id];
	graphicsObjectData.acceptsMouseEvents = enabled;
}

bool ViewLayer::listenMouseEvents() const noexcept
{
	return _listenMouseEvents;
}

void ViewLayer::setListenMouseEvents(bool listenMouseEvents) noexcept
{
	_listenMouseEvents = listenMouseEvents;
}

bool ViewLayer::needConstUpdates() const noexcept
{
	return _needConstUpdates;
}

void ViewLayer::enableConstUpdates(bool needConstUpdates) noexcept
{
	_needConstUpdates = needConstUpdates;
}

void ViewLayer::setSize(const Size<uint32_t>& size) noexcept
{
	onSizeEvent({ size.width, size.height });
}

const Size<uint32_t>& ViewLayer::size() const noexcept
{
	return _layerSize;
}

std::vector<GraphicsObject*> ViewLayer::selectGraphicsObjects(const sf::Rect<float>& rectSpace) const
{
	auto selectedObjects = _aabbTree.query({
		{static_cast<double>(rectSpace.left), static_cast<double>(rectSpace.top)},
		{static_cast<double>(rectSpace.left + rectSpace.width), static_cast<double>(rectSpace.top + rectSpace.height)}
	});

	std::vector<GraphicsObject*> result;
	result.reserve(selectedObjects.size());

	for (auto id : selectedObjects) {
		result.push_back(_objectsMap.at(static_cast<ObjectId>(id)).graphicsObject);
	}

	return result;
}

ViewLayer::~ViewLayer()
{
	std::unordered_map<ObjectId, GraphicsObjectData> objectsMap{ std::move(_objectsMap) };
	_objectsMap = {};

	for ( auto& [_, graphicsObjectData] : objectsMap ) {
		graphicsObjectData.graphicsObject->removeFromViewLayer();
	}
}

void ViewLayer::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	for( auto& [_, objectData] : _objectsMap ) {
		objectData.graphicsObject->draw(renderTarget, deltaTime);
	}
}

void ViewLayer::update(double updateFrequency, double timeDeviation) noexcept
{
	_update = true;
	for ( auto& [_, objectData] : _objectsMap ) {
		objectData.graphicsObject->update(updateFrequency, timeDeviation);
	}

	_update = false;
	for (auto objId : _deferredDeletionList) {
		if (containsId(objId)) {
			removeGraphicsObject(_objectsMap.at(objId).graphicsObject);
		}
	}
}

bool ViewLayer::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept
{
	for ( auto id : getGraphicsObjectIdUnderPointList(mouseButtonEvent.x, mouseButtonEvent.y) ) {
		if ( !_objectsMap.at(id).graphicsObject->onMouseButtonEvent(mouseButtonEvent, eventType) ) {
			return false;
		}
	}
	return true;
}

void ViewLayer::onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept
{
	_cursorOverObjectsSetCache.clear();

	for (auto id : getGraphicsObjectIdUnderPointList(mouseMoveEvent.x, mouseMoveEvent.y)) {
		auto& graphicsObjectData = _objectsMap.at(id);
		if ( !graphicsObjectData.shape.empty() ) {

		}

		if ( !graphicsObjectData.acceptsMouseEvents ) {
			continue;
		}

		if ( _cursorOverObjectsSet.find(id) != _cursorOverObjectsSet.end() ) {
			_cursorOverObjectsSetCache.insert(id);
			_cursorOverObjectsSet.erase(id);
			if ( !graphicsObjectData.graphicsObject->onMouseMoveEvent(mouseMoveEvent) ) {
				break;
			}
		}
		else {
			_cursorOverObjectsSetCache.insert(id);
			if ( !graphicsObjectData.graphicsObject->onMouseEnterEvent(mouseMoveEvent) ) {
				break;
			};
		}
	}

	for ( auto id : _cursorOverObjectsSet ) {
		auto& graphicsObjectData = _objectsMap.at(id);
		if (graphicsObjectData.acceptsMouseEvents) {
			graphicsObjectData.graphicsObject->onMouseLeaveEvent(mouseMoveEvent);
		}
	}

	_cursorOverObjectsSetCache.swap(_cursorOverObjectsSet);
}

void ViewLayer::onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept
{
	_aabbTree.setBoxSize({ static_cast<double>(sizeEvent.width), static_cast<double>(sizeEvent.height) });
	_layerSize.width = sizeEvent.width;
	_layerSize.height = sizeEvent.height;
}

ObjectId ViewLayer::getId() noexcept
{
	if ( !_freeIdList.empty() ) {
		auto id = _freeIdList.back();
		_freeIdList.pop_back();
		return id;
	}

	auto id = _notUsedId;
	++_notUsedId;
	return id;
}

void ViewLayer::releaseId(ObjectId id) noexcept
{
	_freeIdList.push_back(id);
}

bool ViewLayer::containsId(ObjectId id) const noexcept
{
	return _objectsMap.find(id) != _objectsMap.end();
}

ObjectId ViewLayer::checkGraphicsObject(GraphicsObject* graphicsObject) const noexcept
{
	if (!graphicsObject) {
		return 0;
	}

	auto id = graphicsObject->id();

	if (!id || graphicsObject->ownerLayer() != this || !containsId(id)) {
		return 0;
	}

	return id;
}

void ViewLayer::updateGraphicsObjectAabb(ObjectId objectId, const GraphicsObjectData& graphicsObjectData)
{
	auto objectPosition = graphicsObjectData.graphicsObject->position();
	auto posX = objectPosition.x - graphicsObjectData.rect.left;
	auto posY = objectPosition.y - graphicsObjectData.rect.top;

	_aabbTree.updateParticle(
		objectId,
		{
			posX,
			posY
		},
		{
			posX + graphicsObjectData.rect.width,
			posY + graphicsObjectData.rect.height
		}
	);
}

std::vector<uint32_t> ViewLayer::getGraphicsObjectIdUnderPointList(int x, int y)
{
	return _aabbTree.query({ 
		{static_cast<double>(x - 1), static_cast<double>(y - 1)},
		{static_cast<double>(x + 1), static_cast<double>(y + 1)}
	});
}
