#ifndef __MAP_BEHAVIOR__
#define __MAP_BEHAVIOR__

#include "./map.hpp"
#include "../graphics/behavior.hpp"

#include <vector>

struct MapBehavior : public gl::behavior
{
    Map map;
    int districts;

    std::vector<sf::Color> colors;

    bool show_borders;

    MapBehavior(int districts = 3);

    void handle_event(gl::engine&, const sf::Event&);
    void execute(gl::engine&);
};

#endif