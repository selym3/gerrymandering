#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "../util/vec2.hpp"
#include "../util/Random.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

namespace gm
{

// struct district
// {
//     std::vector<node> nodes;
// };

struct node
{
    vec2d pos;
    int party;
};

struct map
{
    std::vector<node> nodes;

    // map generators
    static map from_rectangle(int width, int height);

    // constructors
    map(const std::vector<node>& nodes = {}); 

    // districting algos
    void randomize(int districts);

    // fairness algos
};

}

#endif