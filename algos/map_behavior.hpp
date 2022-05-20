#ifndef __MAP_BEHAVIOR_HPP__
#define __MAP_BEHAVIOR_HPP__

#include "../graphics/behavior.hpp"
#include "./map.hpp"

#include <SFML/Graphics/Color.hpp>

namespace gm
{

struct map_behavior : public gl::behavior 
{
    map _map;

    int districts;
    std::vector<sf::Color> district_colors;

    void execute(gl::engine& en);
    void handle_event(gl::engine& en, const sf::Event& event);

    map_behavior();
};

}

#endif
