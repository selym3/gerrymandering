#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace gl
{

class engine
{
private:
    sf::RenderWindow window;

public:
    engine(unsigned int width, unsigned int height);

private:
    void handle_event(const sf::Event& event);
    void draw();

public:
    bool is_running() const;
    void execute();
};

}

#endif