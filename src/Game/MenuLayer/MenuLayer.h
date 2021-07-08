#pragma once

#include <ViewLayer/ViewLayer.h>
#include <Controls/Button/Button.h>

class MenuLayer : public ViewLayer
{
public:
	MenuLayer();

	virtual void draw(sf::RenderTarget& renderTarget, double deltaTime) noexcept;

	virtual void onSizeEvent(const sf::SizeEvent& sizeEvent) noexcept override;

	virtual ~MenuLayer() override;

	void setStartEventHandler(Button::CallbackType&& eventHandler);

	void setTitleText(const std::string& text);

private:
	void setButtonPosition();
	void setTextPosition();

private:
	Button* _startButton;
	sf::Text _text;
};

