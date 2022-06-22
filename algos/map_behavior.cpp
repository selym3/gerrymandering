#include "./map_behavior.hpp"

using namespace gm;
using namespace gl;

#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

MapBehavior::MapBehavior(int districts) :
    map { Map::make_grid(200, 200) }, 
    districts { districts },
    colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow },
    show_borders { false },
    mode { DrawMode::Districts }
{
    map.reset(districts);

    max_population = -1;
    for (const auto& [pos, node] : map.get_node_map())
    {
        if (node.get_population() > max_population)
            max_population = node.get_population();
    }
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

void MapBehavior::draw_districts(engine& e)
{
    // draw the entire map
    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        sf::Color color = sf::Color{100, 100, 100};
        if (show_borders && map.is_border(pos))
            color = sf::Color::Black;
        else if (n.get_district() >= 0 && n.get_district() < districts)
            color = colors[n.get_district()];

        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);
}

void MapBehavior::draw_hovered(engine& e)
{
    // draw the hovered cell
    std::vector<sf::Vertex> hovered;
    vec2i mouse_cell = get_mouse_cell(e);
    if (map.has_node(mouse_cell)) draw_cell(e, hovered, mouse_cell, sf::Color::Black, true);
    e.get_window().draw(hovered.data(), hovered.size(), sf::LineStrip);
}

void MapBehavior::draw_density(engine& e)
{
    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        double peep = 255 - (255.0 * n.get_population()) / max_population;
        int poop = static_cast<int>(peep);

        sf::Color color = sf::Color{poop, poop, poop};
        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);
}

void MapBehavior::execute(engine& e)
{
    bool space_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    for (int i = 0; !space_pressed && i < 64; ++i) map.evolve();

    switch (mode)
    {
    case DrawMode::Districts: 
        draw_districts(e); 
        break;
    case DrawMode::Density: 
        draw_density(e); 
        break;
    default:
        mode = DrawMode::Districts;
        break;
    };

    draw_hovered(e);
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
        else if (event.key.code == sf::Keyboard::D)
        {

            mode = static_cast<DrawMode>(((static_cast<std::size_t>(mode))+1)%static_cast<std::size_t>(DrawMode::Count));
        }
    }

    else if (event.type == sf::Event::MouseButtonPressed)
    {
        auto pos = get_mouse_cell(e);
        auto node = map.get_node(pos);
        auto btn = event.mouseButton.button;

        if (node.has_value())
            std::cout << node->get().get_population() << "\n";

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            if (btn == sf::Mouse::Left)
            {
                if (node.has_value())
                {
                    District d1 = node->get().get_district();
                    node->get().set_district((node->get().get_district()+1)%districts);

                    map.metric.move_node(pos, node->get(), d1, node->get().get_district());
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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            if (btn == sf::Mouse::Left)
            {
                map.update_border_one(pos);
            }
        }
    }
}