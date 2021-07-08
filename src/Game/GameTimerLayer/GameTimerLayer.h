#pragma once

#include <ViewLayer/ViewLayer.h>
#include <functional>
#include <SFML/Graphics.hpp>
#include <cmath>

class GameTimerLayer : public ViewLayer
{
public:
	using CallbackType = std::function<void()>;

	GameTimerLayer();

	void setTime(double time);
	void setTimeoutEventHandler(CallbackType callback);

public:
	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	virtual void update(double updateFrequency, double timeDeviation) noexcept;

	virtual void onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept override;

private:
	void updateTimerText();
	void updateTimerPosition();

private:
	CallbackType _callback;
	double _time;
	sf::Text _timeText;
	sf::RectangleShape _rect;
};

