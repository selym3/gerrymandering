#include "./map_behavior.hpp"

using namespace gm;
using namespace gl;

#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

MapBehavior::MapBehavior(int districts) :
    map { Map::make_grid(100, 100) }, 
    districts { districts },
    colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green }
{
}

void MapBehavior::draw_cell(engine& e, std::vector<sf::Vertex>& vertices, const vec2i& pos, sf::Color color, bool loop) const
{
    vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y}).to<float>(), color));
    vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y - 1}).to<float>(), color));
    vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x + 1, pos.y - 1}).to<float>(), color));
    vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x + 1, pos.y}).to<float>(), color));
    if (loop)
        vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y}).to<float>(), color));
}

vec2i MapBehavior::get_mouse_cell(const engine& e) const
{
    vec2d mouse_world = e.get_camera().screen_to_world(vec2d::from(sf::Mouse::getPosition(e.get_window())));
    vec2i mouse_cell { static_cast<int>(mouse_world.x), static_cast<int>(mouse_world.y + 1) };
    return mouse_cell;
}

void MapBehavior::execute(engine& e)
{
    // fast evolve (change to a toggle)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
       map.evolve();

    // draw the entire map
    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        sf::Color color = sf::Color{100, 100, 100};
        if (show_borders && map.is_border(pos))
            color = sf::Color::Black;
        else if (n.district >= 0 && n.district < districts)
            color = colors[n.district];

        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);

    // draw the hovered cell
    std::vector<sf::Vertex> hovered;
    vec2i mouse_cell = get_mouse_cell(e);
    if (map.has_node(mouse_cell)) draw_cell(e, hovered, mouse_cell, sf::Color::Black, true);
    e.get_window().draw(hovered.data(), hovered.size(), sf::LineStrip);
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
            map.evolve();
        }
        else if (event.key.code == sf::Keyboard::R) 
        {
            map.find_borders();
        }
    }

    else if (event.type == sf::Event::MouseButtonPressed)
    {
        auto pos = get_mouse_cell(e);
        auto node = map.get_node(pos);

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            return;

        auto btn = event.mouseButton.button;
        if (btn == sf::Mouse::Left)
        {
            if (node.has_value())
            {
                node->get().district++;
                node->get().district %= districts;

                map.update_border(pos);
            }
        }
        else if (btn == sf::Mouse::Right)
        {
            if (node.has_value())
            {
                map.evolve(pos);
            }
        }
    }
}