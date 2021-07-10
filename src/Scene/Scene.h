#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <BaseTypes.h>

class ViewLayer;

class Scene
{
public:
	Scene(Size<uint32_t> sceneSize);

	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator = (const Scene&) = delete;
	Scene& operator = (Scene&&) = delete;

	void pushViewLayer(ViewLayer* viewLayer) noexcept;
	void popViewLayer() noexcept;
	ViewLayer* topViewLayer() const noexcept;

	void drawScene(sf::RenderTarget& renderTarget, double deltaTime) noexcept;
	void updateScene(double updateFrequency, double timeDeviation) noexcept;
	void propagateMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent);
	void propagateMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent, sf::Event::EventType eventType);
	void propagateSizeEvent(const sf::SizeEvent& sizeEvent);

private:
	std::vector<ViewLayer*> _viewLayerStack;
	Size<uint32_t> _sceneSize;
};

