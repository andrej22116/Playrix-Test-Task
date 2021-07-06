#include "Button.h"

Button::Button(const std::string& text)
	: _borderWeight(1)
	, _mouseOver(false)
{
	_font.loadFromFile("./src/fonts/13888.otf");
	_padding[0] = 20;
	_padding[1] = 40;
	_padding[2] = 20;
	_padding[3] = 40;

	_text.setFont(_font);
	_text.setString(text);

	recalculateTextSize();

	enableMouseEvents(true);
}

sf::Rect<float> Button::rect() const noexcept
{
	return { 
		0, 0,
		_padding[1] + _padding[3] + _textSize.width - _borderWeight * 0.5f,
		_padding[0] + _padding[2] + _textSize.height - _borderWeight * 0.5f
	};
}

void Button::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	auto rectWidth = _padding[1] + _padding[3] + _textSize.width - _borderWeight * 0.5f;
	auto rectHeight = _padding[0] + _padding[2] + _textSize.height - _borderWeight * 0.5f;
	auto thisPos = this->position();

	sf::RectangleShape rect{ {rectWidth, rectHeight} };
	rect.setPosition(thisPos);
	rect.setFillColor({ 0,0,0,0 });
	rect.setOutlineColor({ 255, 255, 255 });
	rect.setOutlineThickness(_borderWeight);

	_text.setPosition(thisPos.x + _padding[3], thisPos.y + _padding[0]);

	renderTarget.draw(rect);
	renderTarget.draw(_text);
}

bool Button::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent) noexcept
{
	return false;
}

bool Button::onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept
{
	return false;
}

bool Button::onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept
{
	return false;
}

void Button::recalculateTextSize() noexcept
{
	auto size = _text.getGlobalBounds();
	_textSize.width = size.width;
	_textSize.height = size.height;
}
