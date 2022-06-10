#ifndef __NODE_HPP__
#define __NODE_HPP__

namespace gm
{

using District = int;

struct Node
{
    District district;
    int population;
    std::unordered_map<Party, int> populations_of_each_party;
};
}

#endif