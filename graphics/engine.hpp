#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include "./behavior.hpp"
#include "./camera.hpp"
#include "../mouse.hpp"

namespace gl
{

class engine
{
private:
    sf::RenderWindow _window;
    camera _camera;
    Mouse _mouse;

public: 
    camera& get_camera() { return _camera; }
    sf::RenderWindow& get_window() { return _window; }
    Mouse& get_mouse() { return _mouse; }

private:
    std::vector<std::unique_ptr<behavior>> behaviors;

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