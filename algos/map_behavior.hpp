#ifndef __MAP_BEHAVIOR__
#define __MAP_BEHAVIOR__

#include "./map.hpp"
#include "../graphics/behavior.hpp"

#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace gm
{

struct MapBehavior : public gl::behavior
{
private:
    Map map;
    int districts;

    std::vector<sf::Color> colors;

    bool show_borders;

public:
    MapBehavior(int districts);

    void handle_event(gl::engine&, const sf::Event&);
    void execute(gl::engine&);
};

}

#endif