#include "GameTimerLayer.h"
#include <Game/GameSources/GameSources.h>

GameTimerLayer::GameTimerLayer()
	: _callback(nullptr)
	, _time(0)
	, _timeText("0", GameSources::mainFont(), 256)
{
	_timeText.setLetterSpacing(2);
	_timeText.setFillColor({ 0,0,0,64 });
	enableConstUpdates(false);
	_rect.setPosition(0, 0);
	auto viewSize = size();
	_rect.setSize({ static_cast<float>(viewSize.width), static_cast<float>(viewSize.height) });
}

void GameTimerLayer::setTime(double time)
{
	_time = time;
	enableConstUpdates(true);
	updateTimerText();
	updateTimerPosition();
}

void GameTimerLayer::setTimeoutEventHandler(CallbackType callback)
{
	_callback = callback;
}

void GameTimerLayer::draw(sf::RenderTarget& renderTarget, double /*deltaTime*/) noexcept
{
	if ( _time < 10 ) {
		_rect.setFillColor({ 0,0,0,static_cast<uint8_t>(224.0 * ((10.0 - _time) / 10.0)) });
		renderTarget.draw(_rect);
	}
	renderTarget.draw(_timeText);
}

void GameTimerLayer::update(double updateFrequency, double timeDeviation) noexcept
{
	int oldTime = static_cast<int>(_time);
	_time -= updateFrequency + timeDeviation;
	if (_time < oldTime) {
		updateTimerText();
		updateTimerPosition();
	}
	if (_time < 0) {
		enableConstUpdates(false);
		if ( _callback != nullptr ) {
			_callback();
		}
	}
}

void GameTimerLayer::onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept
{
	ViewLayer::onSizeEvent(sizeEvent);
	updateTimerPosition();
	_rect.setSize({ static_cast<float>(sizeEvent.width), static_cast<float>(sizeEvent.height) });
}

void GameTimerLayer::updateTimerText()
{
	_timeText.setString(std::to_string(static_cast<int>(std::ceil(_time))));
}

void GameTimerLayer::updateTimerPosition()
{
	auto textRect = _timeText.getLocalBounds();
	auto layerSize = size();
	_timeText.setPosition(
		(layerSize.width - textRect.width) * 0.5 - textRect.left,
		(layerSize.height - textRect.height) * 0.5 - textRect.top
	);
}
