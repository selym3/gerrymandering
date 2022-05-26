#include "./map_behavior.hpp"

using namespace gm;
using namespace gl;


#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

MapBehavior::MapBehavior(int districts) :
    map { Map::make_grid(100, 100) }, 
    districts { 3 },
    colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green }
{
}

void MapBehavior::execute(engine& e)
{
    std::vector<sf::Vertex> vertices;
    for (const auto &[pos, n] : map.get_nodes())
    {
        sf::Color color = sf::Color{100, 100, 100};
        if (show_borders && map.is_border(n))
            color = sf::Color::Black;
        else if (n.district >= 0 && n.district < districts)
            color = colors[n.district];

        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y - 1}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x + 1, pos.y - 1}).to<float>(), color));
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x + 1, pos.y}).to<float>(), color));
    }

    e.get_window().draw(vertices.data(), vertices.size(), sf::Quads);
}

void MapBehavior::handle_event(engine& e, const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Q) 
        {
            map.reset(districts);
        }
        else if (event.key.code == sf::Keyboard::W)
        {
            show_borders = !show_borders;
        }
        else if (event.key.code == sf::Keyboard::E) 
        {
            map.update_border();
        }
    }
}