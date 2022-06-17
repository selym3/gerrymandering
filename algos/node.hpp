#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <unordered_map>

namespace gm
{

using District = int;
using Party = int;

// TODO: use a party to account for non-voters / kids
struct Node
{
    Node(District district = -1);

    void set_district(District d1);
    District get_district() const;

    int get_population() const;
    int get_population(Party p) const;

    void set_population(Party p, int population);
    void clear_population();

private:
    District district;
    int population;
    std::unordered_map<Party, int> population_map;
};
}

#endif