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
        return district == 0;
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
    }

    void remove_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) { std::cerr << "something is up???\n"; return; }
        population_map[node.district] -= node.population;
    }

    District move_node(const vec2i& pos, const Node& node, District district)
    {
        District old_district = node.district;
        remove_node(pos, node);

        Node moved = node;
        moved.district = district;
        add_node(pos, node);
        return old_district;
    }

public:
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        auto m1 = get_measurement();
        District old = move_node(pos, node, district);
        auto m2 = get_measurement();
        move_node(pos, node, old);

        return m2 > m1;
    }

private:
    double expected_population() const { return 0.0;}

    double get_measurement() const { return 0.0; }

};

}

#endif