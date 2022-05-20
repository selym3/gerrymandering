#include "./map_behavior.hpp"

using namespace gm;
using namespace gl;

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

map_behavior::map_behavior() :
    _map { 100, 100 }
{
}

void map_behavior::execute(engine& e)
{
    std::vector<sf::Vertex> vertices;
    for (const auto& n : _map.nodes)
    {
        sf::Color color = sf::Color::Black;
        switch (n.party)
        {
            case 0: color = sf::Color::Red;   break;
            case 1: color = sf::Color::Green; break;
            case 2: color = sf::Color::Blue;  break;
        }

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
            _map.randomize(3);
        }
    }
}