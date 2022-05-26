#ifndef __SIMPLE_MAP_HPP__
#define __SIMPLE_MAP_HPP__

#include "../util/vec2.hpp"
#include "../util/Random.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

namespace gm
{

struct node
{
    vec2d pos;
    int party;
};

struct simple_map
{
    std::vector<node> nodes;

    // map generators
    static simple_map from_rectangle(int width, int height);

    // constructors
    simple_map(const std::vector<node>& nodes = {}); 

    // districting algos
    void randomize(int districts);

    // fairness algos
};

}

#endif