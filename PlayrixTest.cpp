// PlayrixTest.cpp : Определяет точку входа для приложения.
//
#include <SFML/Graphics.hpp>
#include <string>

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

    sf::ContextSettings contextSettings{};
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    contextSettings.sRgbCapable = true;
    contextSettings.antialiasingLevel = 4;
    contextSettings.depthBits = 24;

    sf::Font font;
    if (!font.loadFromFile("./src/fonts/13888.otf")) {
        return -1;
    }

    sf::RenderWindow window{ sf::VideoMode{1024, 768}, "Playrix test game!", sf::Style::Default, contextSettings };

    sf::Clock clock;

    double time = 0;

    while ( window.isOpen() )
    {
        double deltaTime = static_cast<double>(clock.getElapsedTime().asMicroseconds()) / 1000000;
        time += deltaTime;
        clock.restart();

        sf::Text text{ "Delta time: " + std::to_string(deltaTime), font, 24 };
        sf::Text text2{ "Time: " + std::to_string(static_cast<int>(time)), font, 24 };
        text.setPosition(24, 24);
        text2.setPosition(24, 52);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.draw(text2);
        //window.draw(shape);
        window.display();
    }

    return 0;
}
