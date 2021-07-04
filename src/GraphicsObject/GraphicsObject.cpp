#include "GraphicsObject.h"
#include <ViewLayer/ViewLayer.h>

std::vector<sf::Vector2<float>> GraphicsObject::shape() const noexcept
{
    return {};
}

void GraphicsObject::draw(sf::RenderTarget& /*renderTarget*/) noexcept
{
}

GraphicsObject::GraphicsObject(std::weak_ptr<ViewLayer> owner)
    : _id(0)
    , _position(0, 0)
    , _name("")
    , _ownerLayer(owner)
    , _isValidGeometry(false)
{
}

GraphicsObject::GraphicsObject(sf::Vector2<float> position, std::weak_ptr<ViewLayer> owner)
    : _id(0)
    , _position(position)
    , _name("")
    , _ownerLayer(owner)
    , _isValidGeometry(false)
{
}

float GraphicsObject::x() const noexcept
{
    return _position.x;
}

float GraphicsObject::y() const noexcept
{
    return _position.y;
}

sf::Vector2<float> GraphicsObject::position() const noexcept
{
    return _position;
}

void GraphicsObject::setX(float x) noexcept
{
    _position.x = x;
}

void GraphicsObject::setY(float y) noexcept
{
    _position.y = y;
}

void GraphicsObject::setPosition(sf::Vector2<float> position) noexcept
{
    _position = position;
}

bool GraphicsObject::isValidGeometry() const noexcept
{
    return _isValidGeometry;
}

void GraphicsObject::updateGeometry() noexcept
{
    _isValidGeometry = false;
}

void GraphicsObject::setGeometryState(bool isValidGeometry) noexcept
{
    _isValidGeometry = isValidGeometry;
}

std::string_view GraphicsObject::name() const noexcept
{
    return { _name };
}

void GraphicsObject::setName(std::string&& name)
{
    _name = std::move(name);
}

void GraphicsObject::setName(const std::string_view& name)
{
    _name = std::string{ name };
}

std::weak_ptr<ViewLayer> GraphicsObject::ownerLayer() const
{
    return _ownerLayer;
}

void GraphicsObject::remove() noexcept
{
    if (auto owner = _ownerLayer.lock(); owner != nullptr) {
        owner->removeGraphicsObject(_id);
        _ownerLayer.reset();
        _id = 0;
    }
}

ObjectId GraphicsObject::id() const noexcept
{
    return _id;
}

void GraphicsObject::setId(ObjectId id) noexcept
{
    _id = id;
}

void setOwnerLayer(std::shared_ptr<GraphicsObject> graphicsObject, std::shared_ptr<ViewLayer> ownerLayer)
{
    graphicsObject->remove();
    ObjectId objectId = ownerLayer->appendGraphicsObject(graphicsObject);
    graphicsObject->setId(objectId);
    graphicsObject->_ownerLayer = ownerLayer;
}
