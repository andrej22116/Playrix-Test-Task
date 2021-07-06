#pragma once

#include <cinttypes>
#include <SFML/Window/Event.hpp>

using ObjectId = uint64_t;

namespace sf {
	using MouseButtonEvent = sf::Event::MouseButtonEvent;
	using MouseMoveEvent = sf::Event::MouseMoveEvent;
	using MouseEnterEvent = sf::Event::MouseMoveEvent;
	using MouseLeaveEvent = sf::Event::MouseMoveEvent;
	using SizeEvent = sf::Event::SizeEvent;
}

template<typename T> struct Size {
	T width;
	T height;
};
