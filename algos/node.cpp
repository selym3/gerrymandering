#include "./node.hpp"

using namespace gm;


Node::Node(District district) :
    district { district },
    population { 0 },
    population_map {}
{
}

void Node::set_district(District d1)
{
    district = d1;
}

District Node::get_district() const 
{
    return district;
}

int Node::get_population() const
{
    return population;
}

int Node::get_population(Party p) const
{
    auto iter = population_map.find(p);
    if (iter == population_map.end())
        return 0;

    return iter->second;
}

const std::unordered_map<Party, int>& Node::get_party_population_map() const
{
    return population_map;
}

void Node::set_population(Party p, int new_pop) 
{
    population -= get_population(p);
    population_map[p] = new_pop;
    population += new_pop;
}

void Node::clear_population()
{
    population = 0;
    population_map.clear();
}