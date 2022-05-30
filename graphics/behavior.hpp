#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

#include <SFML/Window/Event.hpp>

// #include "./engine.hpp"
#include "../util/vec2.hpp"

namespace gl
{

class engine;

struct behavior
{
    virtual void handle_event(engine& engine, const sf::Event& event) = 0;
    virtual void execute(engine& engine) = 0;

    virtual ~behavior();
};

struct pan_zoom : public behavior
{
public:
    pan_zoom();

public:
    void handle_event(engine& engine, const sf::Event& event);
    void execute(engine& engin);

private:
    gm::vec2d anchor;
};
}

#endif