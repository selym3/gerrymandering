#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "graphics/camera.hpp"
#include "algos/map.hpp"

int main(int argc, char *argv[])
{
    const int WINDOW_SIZE_X = 500;
    const int WINDOW_SIZE_Y = 500;

    const std::string filename = "simulation";

    if (argc == 1)
    {
        std::cout << "Enter in number of iterations when running program" << std::endl;
        exit(-1);
    }

    int iterations = std::stoi(argv[1]);

    gl::camera c{ gm::vec2d{ 500, 500 } };
    sf::RenderTexture rt;
    gm::Map map { gm::Map::make_grid(100, 100) };

    map.reset(3);

    std::vector<sf::Color> colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow };

    std::vector<sf::Vertex> cells;


    for (int frame = 0; frame < iterations; frame++) 
    {
        for (const auto &[pos, n] : map.get_node_map())
        {
            sf::Color color = sf::Color{100, 100, 100};
            
            if (map.is_border(pos))
                color = sf::Color::Black;
            else 
                color = colors[n.get_district()];

            cells.push_back(sf::Vertex(c.world_to_screen({pos.x, pos.y}).to<float>(), color));
            cells.push_back(sf::Vertex(c.world_to_screen({pos.x, pos.y - 1}).to<float>(), color));
            cells.push_back(sf::Vertex(c.world_to_screen({pos.x + 1, pos.y - 1}).to<float>(), color));
            cells.push_back(sf::Vertex(c.world_to_screen({pos.x + 1, pos.y}).to<float>(), color));
        }

        rt.draw(cells.data(), cells.size(), sf::Quads);
            
        std::string append = std::to_string(frame);
        append.insert(0, 5 - append.size(), '0');

        if (!rt.getTexture().copyToImage().saveToFile("animation/anim/" + filename + "0" + append + ".png"))
        {
            std::cout << "ERROR: FILE NOT SAVED!!!!" << std::endl;
        }

        map.evolve();
    }
}