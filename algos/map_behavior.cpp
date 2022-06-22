#include "./map_behavior.hpp"

using namespace gm;
using namespace gl;

#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

#include "../util/make_hsv.hpp"


MapBehavior::MapBehavior(int districts) :
    map { Map::make_grid(200, 200).reset(districts) }, 
    _og { map },
    districts { districts },
    colors {},
    show_borders { false },
    mode { DrawMode::Districts },
    font{},
    
    paused { true } ,
    evolving { true },
    evolver_thread
    {
        [](Map& map, int districts, std::atomic<bool>& paused, std::atomic<bool>& running)
        {
            while (running) if (!paused) map.evolve();
        }, 
        std::ref(map), 
        districts,
        std::ref(paused),
        std::ref(evolving)
    }
{

    max_population = -1;
    for (const auto& [pos, node] : map.get_node_map())
    {
        if (node.get_population() > max_population)
            max_population = node.get_population();
    }

    for (int i = 0; i < districts;++i)
    {
        colors.push_back(hsv(static_cast<int>((i * 360.0)/districts), 1, 1));
    }

    if (!font.loadFromFile("./resources/arial.ttf"))std::cerr << " no font i guess " << std::endl;
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

void MapBehavior::draw_map(engine& e, const Map& map)
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

void MapBehavior::draw_districts(engine& e)
{
    draw_map(e, map);
}

void MapBehavior::draw_start(engine& e)
{
    draw_map(e, _og);
}

void MapBehavior::draw_hovered(engine& e)
{
    // draw the hovered cell
    std::vector<sf::Vertex> hovered;
    vec2i mouse_cell = get_mouse_cell(e);
    if (map.has_node(mouse_cell)) draw_cell(e, hovered, mouse_cell, sf::Color::Black, true);
    e.get_window().draw(hovered.data(), hovered.size(), sf::LineStrip);
}

void MapBehavior::draw_both(engine& e)
{
    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        sf::Color color = sf::Color{100, 100, 100};
        if (show_borders && map.is_border(pos))
            color = sf::Color::Black;
        else if (n.get_district() >= 0 && n.get_district() < districts)
            color = colors[n.get_district()];

        double f = (1.0 * n.get_population()) / max_population;
        color = sf::Color(
            static_cast<sf::Uint8>(color.r * f), 
            static_cast<sf::Uint8>(color.g * f), 
            static_cast<sf::Uint8>(color.b * f)
        );
        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);
}

void MapBehavior::draw_density(engine& e)
{
    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        sf::Uint8 f = static_cast<sf::Uint8>(255 - (255.0 * n.get_population()) / max_population);
        sf::Color color = sf::Color{f, f, f};
        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);
}

void MapBehavior::draw_center(engine& e)
{   

    std::unordered_map<District, vec2i> cnt;
    std::unordered_map<District, int> pop;
    // int total_pop = 0;
    for (const auto &[pos, n] : map.get_node_map())
    {
        if (cnt.count(n.get_district()) == 0) cnt[n.get_district()] = vec2i{0, 0};
        if (pop.count(n.get_district()) == 0) pop[n.get_district()] = 0;
        pop[n.get_district()] += n.get_population();
        cnt[n.get_district()] += n.get_population() * pos;
    }
    for (auto& [d, p] : cnt) p/=pop[d];

    std::vector<sf::Vertex> cells;
    for (const auto &[pos, n] : map.get_node_map())
    {
        sf::Uint8 f = static_cast<sf::Uint8>(255 * std::exp(-std::pow(pos.distance(cnt[n.get_district()]) / 32, 2)));
        sf::Color color = sf::Color{f, f, f};
        draw_cell(e, cells, pos, color);
    }
    e.get_window().draw(cells.data(), cells.size(), sf::Quads);
}

void MapBehavior::draw_metric(engine& e)
{
    auto _to_write = "Using: " + map.get_metric().get_name();
    sf::Text text;

    text.setFont(font);
    text.setString(_to_write);
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);

    e.get_window().draw(text);
}

void MapBehavior::execute(engine& e)
{

    switch (mode)
    {
    case DrawMode::Districts: 
        draw_districts(e); 
        break;
    case DrawMode::Start:
        draw_start(e);
        break;
    case DrawMode::Density: 
        draw_density(e); 
        break;
    case DrawMode::Both:
        draw_both(e);
        break;
    case DrawMode::Center:
        draw_center(e);
        break;

    default:
        mode = DrawMode::Districts;
        break;
    };

    draw_metric(e);
    draw_hovered(e);
}

void MapBehavior::handle_event(engine& e, const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::W)
        {
            show_borders = !show_borders;
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            mode = static_cast<DrawMode>(((static_cast<std::size_t>(mode))+1)%static_cast<std::size_t>(DrawMode::Count));
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            mode = static_cast<DrawMode>(((static_cast<std::size_t>(mode))-1)%static_cast<std::size_t>(DrawMode::Count));
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
            paused = !paused;
        }
    }

    else if (event.type == sf::Event::MouseButtonPressed)
    {
        auto pos = get_mouse_cell(e);
        auto node = map.get_node(pos);
        auto btn = event.mouseButton.button;

        if (node.has_value())
        {
            std::cout << "Node(" << pos << "): \n";
            std::cout << "\tDistrict: " << node->get().get_district() << "\n";
            std::cout << "\tPopulation: " << node->get().get_population() << "\n";
        }
    }
}

MapBehavior::~MapBehavior()
{
    evolving = false;
    evolver_thread.join();
}
