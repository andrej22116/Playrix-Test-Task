#include "Scene.h"
#include <ViewLayer/ViewLayer.h>

void Scene::pushViewLayer(ViewLayer* viewLayer) noexcept
{
	_viewLayerStack.push_back(viewLayer);
}

void Scene::popViewLayer() noexcept
{
	_viewLayerStack.pop_back();
}

ViewLayer* Scene::topViewLayer() const noexcept
{
	return !_viewLayerStack.empty() ? _viewLayerStack.back() : nullptr;
}

void Scene::drawScene(sf::RenderTarget& renderTarget, double deltaTime) noexcept
{
	for ( auto viewLayer : _viewLayerStack ) {
		viewLayer->draw(renderTarget, deltaTime);
	}
}

void Scene::updateScene(double updateFrequency, double timeDeviation) noexcept
{
	for (auto viewLayer : _viewLayerStack) {
		if ( viewLayer->needConstUpdates() ) {
			viewLayer->update(updateFrequency, timeDeviation);
		}
	}
}

void Scene::propagateMouseMoveEvent(const sf::MouseMoveEvent& mouseMoveEvent)
{
	for (auto viewLayer : _viewLayerStack) {
		if ( viewLayer->listenMouseEvents() ) {
			viewLayer->onMouseMoveEvent(mouseMoveEvent);
		}
	}
}

void Scene::propagateMouseButtonEvent(const sf::MouseButtonEvent& mouseButtonEvent)
{
	for (auto viewLayer : _viewLayerStack) {
		if (viewLayer->listenMouseEvents()) {
			viewLayer->onMouseButtonEvent(mouseButtonEvent);
		}
	}
}

Scene::Scene()
{
}
