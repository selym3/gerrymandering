#include "./engine.hpp"

using namespace gl;

//

engine::engine(unsigned int width, unsigned int height) :
    window { sf::VideoMode(width, height), "gerrymandering", sf::Style::None }
{
}

//

void engine::handle_event(const sf::Event& event)
{ 
    if (event.type == sf::Event::Closed)
    {
        window.close();
        return;
    }


}

void engine::draw()
{
    window.clear(sf::Color::White);

    window.display();
}

//

bool engine::is_running() const
{
    return window.isOpen();
}


void engine::execute() 
{
    sf::Event event;
    while (window.pollEvent(event))
    { 
        handle_event(event);
    }

    draw();
}

//