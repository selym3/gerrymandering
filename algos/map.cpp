#include "./map.hpp"

#include <iostream>

using namespace gm;
// Utils
const Random::Engine Map::random = {};
const std::vector<vec2i> Map::to_neighbors = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
const std::vector<vec2i> Map::to_eight_neighbors = { {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1} };

bool Map::will_island(const vec2i& v) const
{
    District this_district = node_map.at(v).get_district();
    size_t count_contiguous_regions = 0;
    bool last_was_this_district = false;

    // Go counterclockwise
    for (const vec2i& neighbor: to_eight_neighbors) {

        if (node_map.count(v + neighbor) && this_district == node_map.at(v + neighbor).get_district()) {
            if (!last_was_this_district) {
                ++count_contiguous_regions;
            }
            last_was_this_district = true;
        } else {
            last_was_this_district = false;
        }

    }

    // You overcounted if your first and last node are the same but separated on the other end
    if (
        node_map.count(v + to_eight_neighbors.front()) && this_district == node_map.at(v + to_eight_neighbors.front()).get_district() && 
        node_map.count(v + to_eight_neighbors.back()) && this_district == node_map.at(v + to_eight_neighbors.back()).get_district() &&
        count_contiguous_regions > 1
    ) {
        --count_contiguous_regions;
    }

    return count_contiguous_regions >= 2;
}

// const Populizer Map::random_population = [](auto a, auto b) { return random.next(0, 50); };
// Factory Methods //

Map::Map(std::unique_ptr<Metric>&& _metric, const Settings& settings, const Grid& grid) 
    : node_map{}, node_layout{}, 
      border_set{}, border_layout{},

      districts { settings.districts }, _rate { settings.accept_bad_rate },
      metric { std::move(_metric) }
{
    for (int y = 0; y < grid.height; ++y) 
    {
        for (int x = 0; x < grid.width; ++x)
        {
            add_node(vec2i{x, y}, Node{ -1 });
        }
    }

    reset(districts);
}

void Map::set_metric(std::unique_ptr<Metric>&& _m)
{
    metric = std::move(_m);
    metric->clear();
    
    for (const auto& [pos, node] : node_map)
    {
        metric->add_node(pos, node);
    }
}

int Map::get_districts() const
{
    return districts;
}

// Map::Map(Map&& rhs)
// {
//     std::cout << "penis " << std::endl;
//     node_map = rhs.node_map;
//     node_layout = rhs.node_layout;
//     metric = std::move(rhs.metric);
//     districts = rhs.districts;
//     _rate = rhs._rate;
// }

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
    return node_map[get_random_node_location()]; 
}

void Map::set_district(const vec2i& pos, District d)
{
    auto node = get_node(pos);
    if (!node.has_value()) return;

    metric->move_node(pos, node->get(), d);
    node->get().set_district(d);
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
            if (node->second.get_district() != iter->second.get_district())
                return true;
        }
    }
    return false;
}


void Map::add_border_one(const vec2i& v)
{
    bool was_inserted = border_set.insert(v).second;

    if (was_inserted) 
    {
        border_layout.push_back(v);
    }
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
    if (calculate_border(v)) 
    {
        add_border_one(v);
    }
    else
    {
        remove_border_one(v);
    }
}

void Map::update_border(const vec2i& v)
{
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j) 
        {
            update_border_one(v + vec2i{i, j});
        }
    }
}

const vec2i& Map::get_random_border_location() const
{
    return border_layout[random.next<std::size_t>(0, border_layout.size())];
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
        if (iter != node_map.end()) neighboring_districts.insert(iter->second.get_district());
    }
    return neighboring_districts;
}

// Initialization

void Map::randomize_grid()
{
    districts = 4;

    for (int i = 0; i < 100; ++i) 
    {
        for (int j = 0; j < 100; ++j) 
        {
            vec2i pos = { i, j };
            auto node = get_node(pos);
            if (!node.has_value()) continue;

            if (i < 50) node->get().set_district((j > 50) ? 0 : 1);
            else        node->get().set_district((j > 50) ? 2 : 3);

            // node->get().set_population(Party{0}, 1); // Map::random.next(0, 50);
            metric->add_node(pos, node->get());
        }
    }
}

void Map::randomize_voronoi(int districts)
{
    this->districts = districts;

    std::vector<vec2i> centroids;
    for (int i = 0; i < districts; ++i)
    {
        centroids.push_back(get_random_node_location());
        node_map[centroids.back()].set_district(i);
    }

    for (auto& [position, node] : node_map) 
    {
        // node.clear_population();

        District min_district = -1;
        double min_distance = std::numeric_limits<double>::max();

        for (const auto& centroid : centroids) 
        {
            double curr_distance = centroid.distance(position);
            if (curr_distance <= min_distance) 
            {
                min_district = node_map[centroid].get_district();
                min_distance = curr_distance;
            }
        }

        if (min_district >= 0) 
        {
            // node.set_population(Party{0}, random.next(0, 50));
            node.set_district(min_district);
            metric->add_node(position, node);
        }
    }

    // reset_metric();
}

const Metric& Map::get_metric() const { return *metric; }

void Map::find_borders()
{
    border_layout.clear();
    border_set.clear();

    for (const auto &pos : node_layout)
    {
        update_border_one(pos);
    }
}

double Map::calculate_population(double distance, double radius, double max_population) const
{
    return 1 + max_population * std::exp(-std::pow(distance/radius, 2));
}

void Map::assign_population(Party p, const vec2i& city, double radius, double max_population)
{
    for (auto& [pos, node]: node_map)
    {
        double population = calculate_population(pos.distance(city), radius, max_population);
        node.set_population(p, node.get_population(p) + population);
    }
}

void Map::assign_population(int cities) 
{   
    while (cities --> 0) 
    {
        auto p = get_random_node_location();
        bool republican = (cities % 2) == 0;
        assign_population(Party{0}, p, 10.0, republican ? 100 : 50);
        assign_population(Party{1}, p, 10.0, republican ? 50 : 100);
    }
}

void Map::clear_population()
{
    for (auto& [pos, node]: node_map)
    {
        node.set_population(Party{0}, 0);
    }
}

void Map::reset(int districts)
{
    metric->clear();
    clear_population();

    assign_population(districts);
    randomize_voronoi(districts);
    // randomize_grid();

    find_borders();
}

// Evolution //

void Map::evolve(const vec2i& v)
{

    if (!is_border(v)) {
        remove_border_one(v);
        return;
    }
    if (will_island(v)) {
        evolve(get_random_border_location());
        return;
    }

    auto districts = get_neighboring_districts(v);
    Node& node = node_map[v];

    for (District district : districts) 
    {
        if (district == node.get_district()) continue;
        

        bool should_flip = metric->analyze(v, node, district);
        double funny_number = random.next(0.0, 1.0);

        if (should_flip || funny_number < _rate) 
        {
            set_district(v, district);
            update_border(v);
        }

        //just try one for now
        break;
    }
}

void Map::evolve() 
{
    // sanity_check();
    evolve(get_random_border_location());
}

void Map::sanity_check()
{
    std::size_t s1 = border_layout.size();
    std::size_t s2 = border_set.size();

    if (s1!=s2) { std::cerr << "UHOH UHOH" << std::endl;}

    for (const auto& node : border_layout)
    {
        if (border_set.count(node) == 0)
            std::cerr << "PEEP POOP" << std::endl;
    }
}