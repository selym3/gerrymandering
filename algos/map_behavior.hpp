#ifndef __MAP_BEHAVIOR__
#define __MAP_BEHAVIOR__

#include "./map.hpp"
#include "../graphics/behavior.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Font.hpp>

#include <vector>

#include <thread>
#include <atomic>


namespace gm
{

struct MapBehavior : public gl::behavior
{
private:
    enum class DrawMode
    {
        Districts,
        Start,
        Density,
        Both,
        Center,

        Count
    };

private:
    void draw_districts(gl::engine&);
    void draw_start(gl::engine&);
    void draw_density(gl::engine&);
    void draw_both(gl::engine&);
    void draw_center(gl::engine&);

    void draw_map(gl::engine&, const Map& map);
    void draw_hovered(gl::engine&);
    void draw_cell(gl::engine&, std::vector<sf::Vertex>&, const vec2i&, sf::Color, bool loop = false) const;

    void draw_info(gl::engine&);

private:
    Map map;
    int districts;
    
    std::atomic<bool> paused;
    std::atomic<bool> running;
    std::atomic<bool> evolving;
    std::thread evolver_thread;

private:
    DrawMode mode;

    const Map::NodeMap _og;
    bool show_borders;
    std::vector<sf::Color> colors;
    int max_population;
    sf::Font font;
    int ticks; 

private:
    vec2i get_mouse_cell(const gl::engine&) const;

public:
    MapBehavior(Map&& map);
    // MapBehavior(const Map::Settings&, const Map::Grid&);
    ~MapBehavior();


    void handle_event(gl::engine&, const sf::Event&);
    void execute(gl::engine&);
};

}

#endif