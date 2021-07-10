#include "Button.h"
#include <Game/GameSources/GameSources.h>

Button::Button(const std::string& text)
	: _backgroundOpacityChangeSpeed(10.0)
	, _backgroundOpacity(0)
	, _mouseOver(false)
{
	_padding[0] = 20;
	_padding[1] = 40;
	_padding[2] = 20;
	_padding[3] = 40;

	_text.setFont(GameSources::font("main_font"));
	_text.setCharacterSize(32);
	_text.setString(text);

	_backgroundRect.setOutlineColor({ 0, 0, 0 });
	_backgroundRect.setOutlineThickness(1);

	recalculateButtonProps();

	enableMouseEvents(true);
}

void Button::setText(const std::string& text) noexcept
{
	_text.setString(text);
	recalculateButtonProps();
}

void Button::setPadding(float top, float right, float bottom, float left) noexcept
{
	_padding[0] = top;
	_padding[1] = right;
	_padding[2] = bottom;
	_padding[3] = left;
	recalculateButtonProps();
}

void Button::setOnClickHandler(std::function<void(const std::string_view&)> callback) noexcept
{
	_callback = callback;
}

sf::Rect<float> Button::rect() const noexcept
{
	auto rectSize = _backgroundRect.getSize();
	return { 
		0, 0,
		rectSize.x,
		rectSize.y
	};
}

void Button::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	auto thisPos = this->position();

	if ( _mouseOver && _backgroundOpacity < 1) {
		_backgroundOpacity += _backgroundOpacityChangeSpeed * deltaTime;
		if (_backgroundOpacity > 1.0) {
			_backgroundOpacity = 1;
		}
	}
	else if ( !_mouseOver && _backgroundOpacity > 0 ) {
		_backgroundOpacity -= _backgroundOpacityChangeSpeed * deltaTime;
		if (_backgroundOpacity < 0) {
			_backgroundOpacity = 0;
		}
	}

	uint8_t opacity = static_cast<uint8_t>(std::round(255.0 * _backgroundOpacity));
	uint8_t textColor = 0 + opacity;

	_backgroundRect.setPosition(thisPos);
	_backgroundRect.setFillColor({ 0,0,0, opacity });

	_text.setPosition(thisPos.x + _textOffset.x, thisPos.y + _textOffset.y);
	_text.setFillColor({ textColor, textColor, textColor });

	renderTarget.draw(_backgroundRect);
	renderTarget.draw(_text);
}

bool Button::onMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType) noexcept
{
	if (mouseButtonEvent.button == sf::Mouse::Button::Left 
		&& eventType == sf::Event::MouseButtonPressed
		&& _callback != nullptr ) {
		_callback(name());
	}
	return false;
}

bool Button::onMouseEnterEvent(const sf::MouseEnterEvent& mouseMoveEvent) noexcept
{
	_mouseOver = true;
	return false;
}

bool Button::onMouseLeaveEvent(const sf::MouseLeaveEvent& mouseMoveEvent) noexcept
{
	_mouseOver = false;
	return false;
}

void Button::recalculateButtonProps() noexcept
{
	auto textRect = _text.getGlobalBounds();
	auto rectWidth = _padding[1] + _padding[3] + textRect.width - _backgroundRect.getOutlineThickness() * 0.5f;
	auto rectHeight = _padding[0] + _padding[2] + textRect.height - _backgroundRect.getOutlineThickness() * 0.5f;

	_backgroundRect.setSize({ rectWidth, rectHeight });

	_textOffset.x = _padding[3] - textRect.left;
	_textOffset.y = _padding[0] - textRect.top;
}
