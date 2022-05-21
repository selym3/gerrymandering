#include "./map.hpp"

using namespace gm;

map map::from_rectangle(int width, int height)
{
    map _m;

    auto rnd = Random::getEngine<int>(0, 3);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            _m.nodes.push_back({vec2d{j, i}, -1});
        }
    }
    return _m;
}

map::map(const std::vector<node> &nodes) : nodes{nodes}
{
}

void map::randomize(int districts)
{
    std::vector<node> centroids;
    auto rnd = Random::getEngine<int>(0, nodes.size());
    for (int i = 0; i < districts; ++i)
    {
        auto j = rnd();
        nodes[j].party = i;
        centroids.push_back(nodes[j]);
    }

    for (auto &node : nodes)
    {
        double min_distance = std::numeric_limits<double>::max();
        int min_party = -1;

        for (const auto &centroid : centroids)
        {
            if (&centroid == &node)
            {
                min_party = -1;
                break;
            }

            double distance = node.pos.distance(centroid.pos);
            if (distance < min_distance)
            {
                min_distance = distance;
                min_party = centroid.party;
            }
        }

        if (min_party >= 0)
            node.party = min_party;
    }
}