#ifndef __MAP_BEHAVIOR__
#define __MAP_BEHAVIOR__

#include "./map.hpp"
#include "../graphics/behavior.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <vector>

namespace gm
{

struct MapBehavior : public gl::behavior
{
private:
    enum DrawMode 
    {
        Districts,
        Density,
        Both,
        Center,

        Count
    };

private:
    void draw_districts(gl::engine&);
    void draw_density(gl::engine&);
    void draw_both(gl::engine&);
    void draw_center(gl::engine&);

    void draw_hovered(gl::engine&);
    void draw_cell(gl::engine&, std::vector<sf::Vertex>&, const vec2i&, sf::Color, bool loop = false) const;

private:
    Map map;
    int districts;

private:
    DrawMode mode;

    bool show_borders;
    std::vector<sf::Color> colors;
    int max_population;

private:
    vec2i get_mouse_cell(const gl::engine&) const;

public:
    MapBehavior(int districts);

    void handle_event(gl::engine&, const sf::Event&);
    void execute(gl::engine&);
};

}

#endif