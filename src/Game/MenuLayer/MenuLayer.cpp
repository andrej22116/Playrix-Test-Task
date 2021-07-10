#include "MenuLayer.h"
#include <Game/GameSources/GameSources.h>

MenuLayer::MenuLayer()
	: _text("You are loose", GameSources::font("info_font"), 56)
{
	_text.setFillColor({ 0,0,0 });
	_startButton = new Button("Start new game!");
	_startButton->setInViewLayer(this);
	setButtonPosition();
}

void MenuLayer::draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	renderTarget.draw(_text);

	ViewLayer::draw(renderTarget, deltaTime);
}

void MenuLayer::onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept
{
	ViewLayer::onSizeEvent(sizeEvent);
	setButtonPosition();
	setTextPosition();
}

MenuLayer::~MenuLayer()
{
	if (_startButton) {
		delete _startButton;
		_startButton = nullptr;
	}
}

void MenuLayer::setStartEventHandler(Button::CallbackType&& eventHandler)
{
	_startButton->setOnClickHandler(std::forward<Button::CallbackType>(eventHandler));
}

void MenuLayer::setTitleText(const std::string& text)
{
	_text.setString(text);
	setTextPosition();
}

void MenuLayer::setButtonPosition()
{
	auto buttonRect = _startButton->rect();
	auto& layerSize = size();

	float posX = (layerSize.width - buttonRect.width) * 0.5;
	float posY = (layerSize.height - buttonRect.height) * 0.5;

	_startButton->setPosition({ posX, posY });
}

void MenuLayer::setTextPosition()
{
	auto textRect = _text.getLocalBounds();
	auto& layerSize = size();

	float posX = (layerSize.width - textRect.width) * 0.5 - textRect.left;
	float posY = layerSize.height * 0.25 - textRect.height * 0.5 - textRect.top;

	_text.setPosition({ posX, posY });
}
