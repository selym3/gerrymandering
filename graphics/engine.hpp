#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include "./behavior.hpp"
#include "./camera.hpp"
#include "../util/mouse.hpp"

namespace gl
{

class engine
{
private:
    sf::RenderWindow _window;
    camera _camera;
    Mouse _mouse;

public: 
    camera& get_camera();
    sf::RenderWindow& get_window();
    Mouse& get_mouse();

private:
    std::vector<std::unique_ptr<behavior>> behaviors;

public:
    engine& add_behavior(std::unique_ptr<behavior>&& bhv);
    
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