#ifndef __NODE_HPP__
#define __NODE_HPP__

namespace gm
{

using District = int;
using Party = int;

struct Node
{
    District district;
    int population;
    std::unordered_map<Party, int> population_map;

    // int get_population() const { return population; }
    // int get_population(Party p) const { return population_map[p]; }
};
}

#endif