#pragma once

#include <memory>
#include <set>
#include <BaseTypes.h>

class GraphicsObject;

class ViewLayer
{
public:
	ObjectId appendGraphicsObject(std::shared_ptr<GraphicsObject> graphicsObject);

	void removeGraphicsObject(std::shared_ptr<GraphicsObject> graphicsObject);
	void removeGraphicsObject(ObjectId graphicsObjectId);


private:
	std::set<std::shared_ptr<GraphicsObject>> _childObjectSet;
};

