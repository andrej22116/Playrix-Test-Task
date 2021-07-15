// PlayrixTest.cpp : Определяет точку входа для приложения.
//
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>

#include <Scene/Scene.h>

#include <Game/GameSources/GameSources.h>

#include <Game/MenuLayer/MenuLayer.h>
#include <Game/GameLayer/GameLayer.h>
#include <Game/BackgroundLayer/BackgroundLayer.h>
#include <Game/GameTimerLayer/GameTimerLayer.h>

#include "framework.h"
#include "PlayrixTest.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (std::filesystem::exists("./src/config.json")) {
        GameSources::initSources(std::string{ "./src/config.json" });
    }
    else {
        GameSources::initSources();
    }

    sf::ContextSettings contextSettings{};
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    contextSettings.sRgbCapable = true;
    contextSettings.antialiasingLevel = 4;
    contextSettings.depthBits = 24;

    sf::RenderWindow window{ sf::VideoMode{1024, 768}, "Playrix test game!", sf::Style::Default, contextSettings };
    window.setFramerateLimit(90);

    sf::Clock clock;

    double time = 0;

    auto windowSize = window.getSize();

    auto backgroundLayer = new BackgroundLayer();

    auto menuLayer = new MenuLayer();
    menuLayer->setListenMouseEvents(true);

    auto timerLayer = new GameTimerLayer();

    auto gameLayer = new GameLayer();

    auto scene = new Scene({ windowSize.x, windowSize.y });
    scene->pushViewLayer(backgroundLayer);
    scene->pushViewLayer(menuLayer);

    double constUpdateTime = 0.015;
    double currentConstUpdateTime = 0.0160000001;

    auto endGame = [&](const std::string& msg) {
        timerLayer->setTimeoutEventHandler(nullptr);
        scene->popViewLayer();
        scene->popViewLayer();

        scene->pushViewLayer(menuLayer);
        menuLayer->setTitleText(msg);

        window.setMouseCursorVisible(true);
    };

    menuLayer->setStartEventHandler([&](const std::string_view& str) {
        scene->popViewLayer();

        timerLayer->setTime(60);
        timerLayer->setTimeoutEventHandler(std::bind(endGame, std::string{ "You've lost!" }));

        scene->pushViewLayer(timerLayer);
        scene->pushViewLayer(gameLayer);

        gameLayer->initGame();
        gameLayer->setEndGameHandler(std::bind(endGame, std::string{ "You've won!" }));

        window.setMouseCursorVisible(false);
    });

    clock.restart();
    while ( window.isOpen() )
    {
        double deltaTime = static_cast<double>(clock.getElapsedTime().asMicroseconds()) / 1000000;
        currentConstUpdateTime += deltaTime;

        time += deltaTime;
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:  {
                    window.close();
                    break;
                }
                case sf::Event::MouseMoved: {
                    scene->propagateMouseMoveEvent(event.mouseMove);
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    scene->propagateMouseButtonEvent(event.mouseButton, sf::Event::MouseButtonPressed);
                    break;
                }
                case sf::Event::Resized: {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    scene->propagateSizeEvent(event.size);
                    break;
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        if (currentConstUpdateTime > constUpdateTime) {
            scene->updateScene(constUpdateTime, currentConstUpdateTime - constUpdateTime);
            currentConstUpdateTime = 0;
        }
        scene->drawScene(window, deltaTime);
        window.display();
    }

    return 0;
}
