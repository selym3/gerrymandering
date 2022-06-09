#ifndef __METRIC_HPP__
#define __METRIC_HPP__

#include <iostream>
#include <unordered_map>

#include "../util/vec2.hpp"
#include "./node.hpp"

namespace gm
{

/*
struct Metric
{
    virtual add_node(const vec2i& pos, const Node& node) = 0;
    virtual remove_node(const vec2i& pos, const Node& node) = 0;

    District move_node(const vec2i& pos, const Node& node, District district)
    {
        District old_district = node.district;
        remove_node(pos, node);

        Node moved = node;
        moved.district = district;
        add_node(pos, node);
        return old_district;
    }

    bool analyze(const vec2i& pos, const Node& node, District district) 
    {
        auto m1 = get_measurement();
        District old = move_node(pos, node, district);
        auto m2 = get_measurement();
        move_node(pos, node, old);

        return m2 > m1;
    }

    virtual Comparable get_measurement() = 0;
};

*/

struct NoMetric
{
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        return true;
    }
};

struct BiasedMetric
{
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        return node.district < district;
    }
};

struct PopulationMetric 
{
private:
    std::unordered_map<District, int> population_map;
    int total_population;

    bool contains(District d) const { return population_map.find(d) != population_map.end(); }

public:
    PopulationMetric() : 
        total_population {0},
        population_map {}
    {
    }

public:
    void add_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) population_map[node.district] = 0;
        population_map[node.district] += node.population;
        
        // std::cout << "CHANGING POP" << std::endl;
        total_population += node.population;
    }

    void remove_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) { std::cerr << "something is up???\n"; return; }
        population_map[node.district] -= node.population;

        // std::cout << "CHANGING POP" << std::endl;
        total_population -= node.population;

    }

    void move_node(const vec2i& pos, const Node& node, District d1, District d2)
    {
        Node temp = node;
        temp.district = d1;
        remove_node(pos, temp);

        temp.district = d2;
        add_node(pos, temp);
    }

    void clear() 
    { 
        total_population = 0;
        population_map.clear();
    }

public:
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        auto m1 = get_measurement(district, node.district);
        // std::cout << m1 << " ";
        move_node(pos, node, node.district, district);
        auto m2 = get_measurement(district, node.district);
        // std::cout << m2 << "\n";
        move_node(pos, node, district, node.district);

        // std::cout << "total pop: " << total_population << std::endl;
        std::cout << "expected pop: " << expected_population() << std::endl;
        std::cout << "pop data: " << total_population << std::endl;
        for (auto& [district, pop] : population_map)
        {
            std::cout << district << " " << pop << std::endl;
        }

        return m2 > m1;
    }

private:
    const int DISTRICTS = 4; //TODO

    double expected_population() const { return static_cast<double>(total_population) / DISTRICTS; }

    double get_measurement(const District &d1, const District &d2)
    { 
        double e1 = std::abs((expected_population() - population_map[d1])  / expected_population());
        double e2 = std::abs((expected_population() - population_map[d2])  / expected_population());
        
        return 1.0 - (e1 + e2) / 2.0; 
    }

};

// using Party = int;
// struct PartyPopulationMetric 
// {
// private:
//     std::unordered_map<District, int> party_population_map;
//     std::unordered_map<Party, int> total_party_populations;

//     bool contains(District d) const { return party_population_map.find(d) != party_population_map.end(); }

// public:
//     PopulationMetric() : 
//         total_population {0},
//         population_map {}
//     {
//     }

// public:
//     void add_node(const vec2i& pos, const Node& node)
//     {
//         if (!contains(node.district)) population_map[node.district] = 0;
//         population_map[node.district] += node.population;
//     }

//     void remove_node(const vec2i& pos, const Node& node)
//     {
//         if (!contains(node.district)) { std::cerr << "something is up???\n"; return; }
//         population_map[node.district] -= node.population;
//     }

//     District move_node(const vec2i& pos, const Node& node, District district)
//     {
//         District old_district = node.district;
//         remove_node(pos, node);

//         Node moved = node;
//         moved.district = district;
//         add_node(pos, moved);
//         return old_district;
//     }

// public:
//     bool analyze(const vec2i& pos, const Node& node, District district)
//     {
//         auto m1 = get_measurement();
//         District old = move_node(pos, node, district);
//         auto m2 = get_measurement();
//         move_node(pos, node, old);

//         return m2 > m1;
//     }

// private:
//     const int DISTRICTS = 3; //TODO

//     double expected_population() const { total_population / DISTRICTS; }

//     double get_measurement(const District &d1, const District &d2) const 
//     { 
//         double e1 = (expected_population() - population_map[d1])  / expected_population;
//         double e2 = (expected_population() - population_map[d2])  / expected_population;
        
//         return (e1 + e2) / 2.0; 
//     }

// };

}

#endif