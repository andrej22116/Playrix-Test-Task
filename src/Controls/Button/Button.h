#pragma once
#include <SFML/Graphics.hpp>
#include <GraphicsObject/GraphicsObject.h>
#include <BaseTypes.h>
#include <functional>

class Button : public GraphicsObject
{
public:
	Button(const std::string& text = "");

	void setText(const std::string& text) noexcept;
	void setPadding(float top, float right, float bottom, float left) noexcept;

	void setOnClickHandler(std::function<void(const std::string_view&)> callback) noexcept;

	virtual sf::Rect<float> rect() const noexcept;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept override;
	virtual bool onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept override;
	virtual bool onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept override;

private:
	void recalculateButtonProps() noexcept;

private:
	float _padding[4];
	sf::Font _font;
	sf::Text _text;
	sf::RectangleShape _backgroundRect;
	sf::Vector2<float> _textOffset;
	std::function<void(const std::string_view&)> _callback;
	sf::Clock _clock;
	float _backgroundOpacityChangeSpeed;
	float _backgroundOpacity;
	bool _mouseOver;
};

