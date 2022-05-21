#ifndef __MAP2_HPP__
#define __MAP2_HPP__

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../util/vec2.hpp"
#include "../util/Random.hpp"

#include "../graphics/engine.hpp"
#include "../graphics/behavior.hpp"

#include <iostream>

namespace gm
{

struct Node
{
    int district;
    bool border;
};

struct Map2
{
    using NodeMap = std::unordered_map<vec2i, Node, vec2i_hash>;
    NodeMap node_map;
    std::vector<vec2i> points;

    std::vector<vec2i> offsets { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    using NodeSet = std::unordered_set<vec2i, vec2i_hash>;
    std::vector<vec2i> border_list;
    NodeSet borders;

    static Map2 make_grid(int width, int height)
    {
        Map2 m;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j) 
            {
                m.add(vec2i{j,i}, Node{-1, false});
            }
        }
        return m;
    }

    Map2() : node_map{}, points{}
    {
    }

    void add(const vec2i& point, const Node& node)
    {
        node_map[point] = node;
        points.push_back(point);
    }

    void randomize(int districts)
    {
        // find random points contained in the map
        std::vector<vec2i> centroids;
        auto rng = Random::getEngine(std::size_t{}, points.size());
        for (int i = 0; i < districts; ++i)
        {
            const vec2i& centroid = points[rng()];
            node_map[centroid].district = i;
            
            centroids.push_back(centroid);
        }

        // iterate through all other
        for (auto &[pos, node] : node_map)
        {
            node.border = false;

            double min_distance = std::numeric_limits<double>::max();
            int min_district = -1;

            for (const auto& centroid : centroids)
            {
                // if (pos == centroid)
                // { min_index = -1; break;}

                double distance = pos.distance(centroid);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    min_district = node_map[centroid].district;
                }
            }

            if (min_district >= 0)
            { node.district = min_district; }
        }
    }

    bool is_border(const vec2i& location) const
    {
        const Node& node = node_map.find(location)->second;

        for (const auto& offset : offsets)
        {
            auto neighbor = node_map.find(location + offset);
            if (neighbor == node_map.end()) continue;

            // there's probably some optimization here that knows to skip the neighbors because
            // if this node is a border, the neighbor is also a border
            if (neighbor->second.district != node.district)
                return true;
        }
        return false;
    }

    void find_borders()
    {
        borders.clear();
        border_list.clear();
        
        for (const auto& point : points)
        {
            if (is_border(point))
            {
                node_map[point].border = true;
                
                borders.insert(point);
                border_list.push_back(point);
            } 
            else 
            {
                node_map[point].border = false;
            }
        }
    }

    void update_border(const vec2i& border_point)
    {
        Node& border_node = node_map[border_point];

        // find districts in the area;
        std::unordered_set<int> district_set;
        for (const auto& offset : offsets)
        {
            auto neighbor = node_map.find(border_point + offset);
            if (neighbor != node_map.end()) 
            {
                if (neighbor->second.district != border_node.district)
                    district_set.insert(neighbor->second.district);
            }
        }
        if (district_set.size() == 0)
        { std::cout << "Something weird going on \n"; return; }

        // choose random district from district set
        auto i = 0; // Random::getEngine(std::size_t{}, district_set.size())();
        int d = -1;
        for (const auto& district : district_set) {
            d = district;
            i--;
            if (i < 0) break;
        }

        if (d == border_node.district) return; // do nothing
        
        std::cout << "Changing " << border_point << " from " << border_node.district << " to " << d<<"\n";
        border_node.district = d;

        // check surrounding nodes to see if they're still borders
        for (const auto& offset : offsets)
        {
            auto neighbor = node_map.find(border_point + offset);
            if (neighbor != node_map.end()) 
            {
                bool still_border = is_border(neighbor->first);
                neighbor->second.border = still_border;
                if (!still_border)
                {

                    // O(N)
                    for (auto begin = border_list.begin(); begin != border_list.end(); ++begin)
                    {
                        if (*begin == neighbor->first)
                        {
                            border_list.erase(begin);
                            break;
                        }
                    }
                    borders.erase(neighbor->first);
                }
            }
        }
    }

    void update_border()
    {
        // choose random border location
        std::size_t random_index = Random::getEngine(std::size_t{}, border_list.size())();
        update_border(border_list[random_index]);
    }

};

struct Map2_behavior : public gl::behavior
{
    Map2 _map;

    int districts;
    std::vector<sf::Color> district_colors;

    bool show_borders = true;

    void execute(gl::engine& e)
    {
        // draw map
        std::vector<sf::Vertex> vertices;
        for (const auto& [pos, n] : _map.node_map)
        {
            sf::Color color = sf::Color{100, 100, 100};
            if (show_borders && n.border) 
                color = sf::Color::Black;
            else if (n.district >= 0 && n.district < districts) 
                color = district_colors[n.district];

            vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y}).to<float>(), color));
            vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x, pos.y-1}).to<float>(), color));
            vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x+1, pos.y-1}).to<float>(), color));
            vertices.push_back(sf::Vertex(e.get_camera().world_to_screen({pos.x+1, pos.y}).to<float>(), color));
        }

        e.get_window().draw(vertices.data(), vertices.size(), sf::Quads);
    }

    void handle_event(gl::engine& e, const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Q)
            {
                // reset map
                _map.randomize(districts);
                _map.find_borders();
            }
            else if (event.key.code == sf::Keyboard::W)
            {
                // show borders?
                show_borders = !show_borders;
            }
            else if (event.key.code == sf::Keyboard::E)
            {
                // progress borders
                _map.update_border();
            }
        }
    }

    Map2_behavior() : 
        _map { Map2::make_grid(100, 100) },
        districts { 3 }, 
        district_colors { sf::Color::Red, sf::Color::Blue, sf::Color::Green }
    {
    }
};



}

#endif