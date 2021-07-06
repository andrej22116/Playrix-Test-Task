#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <BaseTypes.h>
#include <AABB/AABB.h>

class GraphicsObject;

namespace sf {
	class RenderTarget;
}

class ViewLayer
{
	ViewLayer(const ViewLayer&) = delete;
	ViewLayer& operator = (const ViewLayer&) = delete;

public:
	ViewLayer();

	ViewLayer(ViewLayer&& viewLayer) noexcept;
	ViewLayer& operator = (ViewLayer&& viewLayer) noexcept;

	ObjectId appendGraphicsObject(GraphicsObject* graphicsObject);

	void removeGraphicsObject(GraphicsObject* graphicsObject);

	void updateGraphicsObjectGeometry(GraphicsObject* graphicsObject);

	void updateGraphicsObjectPosition(GraphicsObject* graphicsObject);

	void enableSendingMouseEventsToGraphicsObject(GraphicsObject* graphicsObject);

	bool listenMouseEvents() const noexcept;
	void setListenMouseEvents(bool listenMouseEvents) noexcept;

	bool needConstUpdates() const noexcept;
	void enableConstUpdates(bool enableConstUpdates) noexcept;

	void setSize(const Size<uint32_t>& size) noexcept;

	virtual ~ViewLayer();

public:
	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent) noexcept;
	virtual void onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept;

	virtual void onResizeEvent(const sf::SizeEvent& sizeEvent) noexcept;

private:
	struct GraphicsObjectData;

private:
	ObjectId getId() noexcept;
	void releaseId(ObjectId id) noexcept;
	bool containsId(ObjectId id) const noexcept;
	ObjectId checkGraphicsObject(GraphicsObject* graphicsObject) const noexcept;
	void updateGraphicsObjectAabb(ObjectId objectId, const GraphicsObjectData& graphicsObjectData);
	std::vector<uint32_t> getGraphicsObjectIdUnderPointList(int x, int y);

private:
	std::unordered_map<ObjectId, GraphicsObjectData> _objectsMap;
	std::unordered_set<ObjectId> _cursorOverObjectsSet;
	std::unordered_set<ObjectId> _cursorOverObjectsSetCache;
	std::vector<ObjectId> _freeIdList;
	ObjectId _notUsedId;
	ObjectId _underCursorObjectId;
	Size<uint32_t> _layerSize;
	aabb::Tree _aabbTree;
	bool _listenMouseEvents;
	bool _needConstUpdates;
};

