#pragma once
#include <SFML/Graphics.hpp>
#include <GraphicsObject/GraphicsObject.h>
#include <BaseTypes.h>

class Button : public GraphicsObject
{
public:
	Button(const std::string& text = "");

	virtual sf::Rect<float> rect() const noexcept;

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;

	virtual bool onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent) noexcept override;
	virtual bool onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept override;
	virtual bool onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept override;

private:
	void recalculateTextSize() noexcept;

private:
	float _borderWeight;
	float _padding[4];
	sf::Font _font;
	sf::Text _text;
	Size<float> _textSize;
	bool _mouseOver;
};

