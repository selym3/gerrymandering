#include "./map.hpp"

#include <iostream>

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
            map.add_node(vec2i{x, y}, Node{ -1, 0 });
        }
    }

    return map;
}

// Nodes //

bool Map::add_node(const vec2i& v, const Node& node)
{
    if (has_node(v)) return false;
    
    node_layout.push_back(v);
    node_map.insert({v, node});
    return true;
}

bool Map::has_node(const vec2i& v) const { return node_map.find(v) != node_map.end(); }

const std::unordered_map<vec2i, Node, vec2i_hash>& Map::get_node_map() const { return node_map; }

// std::optional<std::reference_wrapper<const Node>> Map::get_node(const vec2i& v) const
// {
//     auto iter = node_map.find(v); // vec2i, Node
//     if (iter == node_map.end()) return std::nullopt;
//     return std::cref(iter->second);
// }

std::optional<std::reference_wrapper<Node>> Map::get_node(const vec2i& v)
{
    auto iter = node_map.find(v); // vec2i, Node
    if (iter == node_map.end()) return std::nullopt;
    return std::ref(iter->second);

//    if (!has_node(v)) return std::nullopt;
//    return node_map[v];
}

const vec2i& Map::get_random_node_location() const 
{
    return node_layout.at(random.next<std::size_t>(0, node_layout.size()));
}

Node& Map::get_random_node() 
{ 
    return node_map.at(get_random_node_location()); 
}

// std::vector<std::reference_wrapper<const Node>> Map::get_neighbors(const vec2i& v) const
// {
//     std::vector<std::reference_wrapper<const Node>> out;
//     for (const auto& to_neighbor : to_neighbors) 
//     {
//         auto maybe_node = get_node(v + to_neighbor);
//         if (maybe_node.has_value()) out.push_back(maybe_node->get());
//     }
//     return out;
// }

// Borders //

bool Map::calculate_border(const vec2i& v) const
{
    auto node = node_map.find(v);
    if (node == node_map.end()) return false;

    for (const auto& to_neighbor : to_neighbors) 
    {
        auto iter = node_map.find(to_neighbor + v);
        if (iter != node_map.end())
        {
            if (node->second.district != iter->second.district)
                return true;
        }
    }
    return false;
}


void Map::add_border_one(const vec2i& v)
{
    std::cout << v << vec2i_hash()(v) << std::endl;
    //std::cout << "Entering border one" << std::endl;
    auto result = border_set.insert(v);
    //std::cout << "Result successfully extracted" << std::endl;
    bool was_inserted = result.second;
    //std::cout << "Starting push back" << std::endl;
    if (was_inserted) border_layout.push_back(v);
    //std::cout << "Finished push back" << std::endl;
}

void Map::remove_border_one(const vec2i& v)
{
    bool was_removed = border_set.erase(v) > 0;
    
    if (!was_removed) return;
    
    // search for vector to remove it from layout vector
    auto it = border_layout.begin();
    for (; it != border_layout.end(); ++it)
        if (*it == v) break;
    border_layout.erase(it);
}

void Map::update_border_one(const vec2i& v)
{   
    std::cout << "actually changing border\n";
    if (calculate_border(v)) 
        add_border_one(v);
    else
        remove_border_one(v);
}

void Map::update_border_helper(std::unordered_set<vec2i, vec2i_hash>& visited, const vec2i& v)
{

    update_border_one(v);
    
    for (const auto& to_neighbor : to_neighbors) 
    {   
       // std::cout << "Entering!" << std::endl;
        if (
            visited.find(v + to_neighbor) != visited.end() && 
            has_node(v + to_neighbor) && 
            calculate_border(v + to_neighbor)
        )
        {   
         //   std::cout << "Made it in" << std::endl;
            // visited.insert(v);
            // update_border_helper(visited, v + to_neighbor);
        }
    }
}

void Map::update_border(const vec2i& v) 
{   
    std::unordered_set<vec2i, vec2i_hash> visited{v};
    update_border_helper(visited, v);
}

const vec2i& Map::get_random_border_location() const
{
    return border_layout.at(random.next<std::size_t>(0, border_layout.size()));
}

bool Map::is_border(const vec2i& v) const
{
    return border_set.find(v) != border_set.end();
}

std::unordered_set<District> Map::get_neighboring_districts(const vec2i& v) const
{
    // remove repeated districts
    std::unordered_set<District> neighboring_districts;
    for (const auto& to_neighbor : to_neighbors) 
    {
        auto iter = node_map.find(to_neighbor + v); // vec2i, Node
        if (iter != node_map.end()) neighboring_districts.insert(iter->second.district);
    }
    return neighboring_districts;
}

// Initialization

void Map::randomize(int districts)
{
    std::vector<vec2i> centroids;
    for (int i = 0; i < districts; ++i)
    {
        centroids.push_back(get_random_node_location());
        node_map.at(centroids.back()).district = i;
    }

    for (auto& [position, node] : node_map) 
    {
        District min_district = -1;
        double min_distance = std::numeric_limits<double>::max();

        for (const auto& centroid : centroids) 
        {
            double curr_distance = centroid.distance(position);
            if (curr_distance <= min_distance) 
            {
                min_district = node_map.at(centroid).district;
                min_distance = curr_distance;
            }
        }

        if (min_district >= 0) 
            node.district = min_district;
    }
}

void Map::find_borders()
{
    border_layout.clear();
    border_set.clear();

    for (const auto &pos : node_layout)
    {
        if (calculate_border(pos))
            add_border_one(pos);
    }
}

void Map::reset(int districts)
{
    randomize(districts);
    find_borders();
}

// Evolution //

void Map::evolve(const vec2i& v)
{
    auto districts = get_neighboring_districts(v);
    Node& node = node_map.at(v);

    for (District district : districts) 
    {
        if (district == node.district) continue;
        
        if (metric.analyze(v, node, district)) 
        {
            node.district = district;
            update_border(v);
        }

        //just try one for now
        break;
    }
}

void Map::evolve() 
{
    evolve(get_random_border_location());
}