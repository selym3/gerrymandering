#include "./engine.hpp"

using namespace gl;

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

camera& engine::get_camera() { return _camera; }
const camera& engine::get_camera() const { return _camera; }

sf::RenderWindow& engine::get_window() { return _window; }
const sf::RenderWindow& engine::get_window() const { return _window; }

Mouse& engine::get_mouse() { return _mouse; }
const Mouse& engine::get_mouse() const { return _mouse; }

engine::engine(unsigned int width, unsigned int height) :
    _window { sf::VideoMode(width, height), "gerrymandering", sf::Style::Default },
    _camera { window{width, height} },
    behaviors {}
{
    behaviors.push_back(std::make_unique<pan_zoom>());

    if (is_running())
    {
        _window.clear(sf::Color::White);
        _window.display();
    }
}

engine& engine::add_behavior(std::unique_ptr<behavior>&& bhv)
{
    behaviors.push_back(std::move(bhv)); 
    return *this; 
}

//

void engine::handle_event(const sf::Event& event)
{ 
    if (event.type == sf::Event::Closed)
    {
        _window.close();
        return;
    }

    for (auto& behavior : behaviors) 
        behavior->handle_event(*this, event);
}

void engine::draw()
{
    _window.clear(sf::Color::White);
    for (auto& behavior : behaviors)
            behavior->execute(*this);
    _window.display();
}

//

bool engine::is_running() const
{
    return _window.isOpen();
}


void engine::execute() 
{
    sf::Event event;
    while (_window.pollEvent(event))
    { 
        handle_event(event);
    }

    draw();
}

//