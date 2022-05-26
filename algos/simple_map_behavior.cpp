#include "./simple_map_behavior.hpp"

using namespace gm;
using namespace gl;

#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

map_behavior::map_behavior() :
    _map { simple_map::from_rectangle(100, 100) }, 
    districts { 3 },
    district_colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green }
{
}

void map_behavior::execute(engine& e)
{
    std::vector<sf::Vertex> vertices;
    for (const auto& n : _map.nodes)
    {
        sf::Color color = sf::Color::Black;
        if (n.party >= 0 && n.party < districts) 
            color = district_colors[n.party];

        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({n.pos.x, n.pos.y}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({n.pos.x, n.pos.y-1}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({n.pos.x+1, n.pos.y-1}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({n.pos.x+1, n.pos.y}).to<float>(), color));
    }

    e.get_window().draw(vertices.data(), vertices.size(), sf::Quads);
}

void map_behavior::handle_event(engine& e, const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Q) 
        {
            _map.randomize(districts);
        }
    }
}