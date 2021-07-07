#pragma once
#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>
#include <BaseTypes.h>

#include <memory>

class ViewLayer;
class GraphicsObject;

class GraphicsObject
{
public:
	GraphicsObject(ViewLayer* owner = nullptr);
	GraphicsObject(sf::Vector2<float> position, ViewLayer* owner = nullptr);

	float x() const noexcept;
	float y() const noexcept;
	sf::Vector2<float> position() const noexcept;

	void setX(float x) noexcept;
	void setY(float y) noexcept;
	void setPosition(sf::Vector2<float> position) noexcept;

	void updateGeometry() noexcept;

	std::string_view name() const noexcept;
	void setName(std::string&& name);
	void setName(const std::string_view& name);

	ViewLayer* ownerLayer() const;
	void removeFromViewLayer() noexcept;

	ObjectId id() const noexcept;
	void setInViewLayer(ViewLayer* viewLayer) noexcept;

	void enableMouseEvents(bool isEnabled) noexcept;
	bool acceptsMouseEvents() const noexcept;

public:
	virtual sf::Rect<float> rect() const noexcept = 0;
	virtual std::vector<sf::Vector2<float>> shape() const noexcept;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept;
	virtual bool onMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent) noexcept;
	virtual bool onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept;
	virtual bool onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept;

	virtual ~GraphicsObject();

private:
	void setId(ObjectId id) noexcept;

private:
	ObjectId _id;
	sf::Vector2<float> _position;
	std::string _name;
	ViewLayer* _ownerLayer;
	bool _acceptMouseEvents;
};

