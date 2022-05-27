#include "./map.hpp"

// #include <iostream>

using namespace gm;

// Factory Methods //

Map::Map() : node_map{}, node_layout{}, border_set{}, border_layout{}, random{},
             to_neighbors { {0, 1}, {0, -1}, {1, 0}, {-1, 0} }
{
}

Map Map::make_grid(int width, int height)
{
    Map map;

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x)
        {
            map.add_node(vec2i{x, y}, Node{ 0, 0 });
        }
    }

    return map;
}

// Nodes //

bool Map::add_node(const vec2i& v, const Node& node)
{
    if (has_node(v)) return false;
    
    node_layout.push_back(v);
    node_map[v] = node;
    return true;
}

bool Map::has_node(const vec2i& v) const { return node_map.find(v) != node_map.end(); }

const std::unordered_map<vec2i, Node, vec2i_hash>& Map::get_node_map() const { return node_map; }

// std::optional<std::reference_wrapper<Node>> Map::get_node(const vec2i& v)
// {
//     auto iter = node_map.find(v); // vec2i, Node
//     if (iter == node_map.end()) return std::nullopt;
//     return std::ref(iter->second);
// }

// std::optional<std::reference_wrapper<const Node>> Map::get_node(const vec2i& v) const
// {
//     auto iter = node_map.find(v); // vec2i, Node
//     if (iter == node_map.end()) return std::nullopt;
//     return std::cref(iter->second);
// }

// Borders //

// bool is_border(const vec2i)

// Initialization

void Map::randomize(int districts)
{
    std::vector<vec2i> centroids;
    for (int i = 0; i < districts; ++i)
    {
        vec2i n = node_layout[random.next(std::size_t{}, node_layout.size())];
        node_map[n].district = i;

        centroids.push_back(n);
    }

    for (auto& [position, node] : node_map) 
    {
        int min_district = -1;
        double min_distance = std::numeric_limits<double>::max();

        for (const auto& centroid : centroids) 
        {
            double curr_distance = centroid.distance(position);
            if (curr_distance <= min_distance) 
            {
                min_district = node_map[centroid].district;
                min_distance = curr_distance;
            }
        }

        if (min_district >= 0) 
            node.district = min_district;
    }
}

void Map::find_borders()
{

}

void Map::reset(int districts)
{
    randomize(districts);
    find_borders();
}