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

    map(int width, int height) 
    {
        auto rnd = Random::getEngine<int>(0, 3);
        for (int i = 0; i < height; ++i) 
        {
            for (int j = 0; j < width; ++j) 
            {
                nodes.push_back({ vec2d{ j, i }, -1});
            }
        }

    }

    void randomize(int districts) 
    {
        std::vector<node> centroids;
        auto rnd = Random::getEngine<int>(0, nodes.size());
        for (int i = 0; i < districts; ++i) 
        {
            auto j = rnd();
            nodes[j].party = i;
            centroids.push_back(nodes[j]);
        }

        for (auto& node : nodes) 
        {
            double min_distance = std::numeric_limits<double>::max();
            int min_party = -1;

            for (const auto& centroid : centroids)
            {
                if (&centroid == &node) 
                {min_party = -1; break; }

                double distance = node.pos.distance(centroid.pos);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    min_party = centroid.party;
                }
            }

            if (min_party >= 0) node.party = min_party;
        }
    }


};

}

#endif