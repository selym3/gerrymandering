#include "./engine.hpp"

using namespace gl;

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

//

struct draw_rectangle : public behavior
{
    draw_rectangle() { }

    void execute(engine& engine)
    {
        sf::RectangleShape rect;
        gm::vec2d rpos{0, 0};
        gm::vec2d rsize{10, 10};
        rect.setPosition(engine.get_camera().world_to_screen(rpos).to<float>());
        rect.setSize((engine.get_camera().world_to_screen(rpos + rsize) - engine.get_camera().world_to_screen(rpos)).to<float>());
        rect.setFillColor(sf::Color::Blue);

        engine.get_window().draw(rect);
    }

    void handle_event(engine& engine, const sf::Event& event) 
    {
    }

};

engine::engine(unsigned int width, unsigned int height) :
    _window { sf::VideoMode(width, height), "gerrymandering", sf::Style::None },
    _camera { window{width, height} },
    behaviors {}
{
    behaviors.push_back(std::make_unique<pan_zoom>());
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