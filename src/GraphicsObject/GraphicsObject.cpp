#include "GraphicsObject.h"
#include <ViewLayer/ViewLayer.h>

std::vector<sf::Vector2<float>> GraphicsObject::shape() const noexcept
{
    return {};
}

void GraphicsObject::draw(sf::RenderTarget& /*renderTarget*/, double /*deltaTime*/) noexcept
{
}

void GraphicsObject::update(double /*updateFrequency*/, double /*timeDeviation*/ ) noexcept
{
}

bool GraphicsObject::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept
{
    return true;
}

bool GraphicsObject::onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept
{
    return true;
}

bool GraphicsObject::onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept
{
    return true;
}

bool GraphicsObject::onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept
{
    return true;
}

GraphicsObject::~GraphicsObject()
{
    removeFromViewLayer();
}

GraphicsObject::GraphicsObject(ViewLayer* owner)
    : _id(0)
    , _position(0, 0)
    , _name("")
    , _ownerLayer(owner)
    , _acceptMouseEvents(false)
{
}

GraphicsObject::GraphicsObject(sf::Vector2<float> position, ViewLayer* owner)
    : _id(0)
    , _position(position)
    , _name("")
    , _ownerLayer(owner)
    , _acceptMouseEvents(false)
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
    if (_ownerLayer) {
        _ownerLayer->updateGraphicsObjectPosition(this);
    }
}

void GraphicsObject::setY(float y) noexcept
{
    _position.y = y;
    if (_ownerLayer) {
        _ownerLayer->updateGraphicsObjectPosition(this);
    }
}

void GraphicsObject::setPosition(sf::Vector2<float> position) noexcept
{
    _position = position;
    if ( _ownerLayer ) {
        _ownerLayer->updateGraphicsObjectPosition(this);
    }
}

void GraphicsObject::updateGeometry() noexcept
{
    if ( _ownerLayer ) {
        _ownerLayer->updateGraphicsObjectGeometry(this);
    }
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

ViewLayer* GraphicsObject::ownerLayer() const
{
    return _ownerLayer;
}

void GraphicsObject::removeFromViewLayer() noexcept
{
    if ( _ownerLayer ) {
        _ownerLayer->removeGraphicsObject(this);
        _ownerLayer = nullptr;
        _id = 0;
    }
}

ObjectId GraphicsObject::id() const noexcept
{
    return _id;
}

void GraphicsObject::setInViewLayer(ViewLayer* viewLayer) noexcept
{
    if ( !viewLayer ) {
        return;
    }

    auto id = viewLayer->appendGraphicsObject(this);
    if ( !id ) {
        return;
    }

    _id = id;
    _ownerLayer = viewLayer;
}

void GraphicsObject::enableMouseEvents(bool isEnabled) noexcept
{
    _acceptMouseEvents = isEnabled;
    if ( _ownerLayer ) {
        _ownerLayer->enableSendingMouseEventsToGraphicsObject(this, isEnabled);
    }
}

bool GraphicsObject::acceptsMouseEvents() const noexcept
{
    return _acceptMouseEvents;
}

void GraphicsObject::setId(ObjectId id) noexcept
{
    _id = id;
}
