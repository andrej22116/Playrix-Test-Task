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
	GraphicsObject(std::weak_ptr<ViewLayer> owner = {});
	GraphicsObject(sf::Vector2<float> position, std::weak_ptr<ViewLayer> owner = {});

	float x() const noexcept;
	float y() const noexcept;
	sf::Vector2<float> position() const noexcept;

	void setX(float x) noexcept;
	void setY(float y) noexcept;
	void setPosition(sf::Vector2<float> position) noexcept;

	bool isValidGeometry() const noexcept;
	void updateGeometry() noexcept;
	void setGeometryState(bool isValidGeometry) noexcept;

	std::string_view name() const noexcept;
	void setName(std::string&& name);
	void setName(const std::string_view& name);

	std::weak_ptr<ViewLayer> ownerLayer() const;
	void remove() noexcept;

	ObjectId id() const noexcept;
	friend void setOwnerLayer(std::shared_ptr<GraphicsObject> graphicsObject, std::shared_ptr<ViewLayer> ownerLayer);

public:
	virtual sf::Rect<float> rect() const noexcept = 0;
	virtual std::vector<sf::Vector2<float>> shape() const noexcept;

	virtual void draw(sf::RenderTarget& renderTarget) noexcept;

	virtual ~GraphicsObject() {}

private:
	void setId(ObjectId id) noexcept;

private:
	ObjectId _id;
	sf::Vector2<float> _position;
	std::string _name;
	std::weak_ptr<ViewLayer> _ownerLayer;
	bool _isValidGeometry;
};

