#include "./graphics/engine.hpp"

#include "./algos/map.hpp"
#include "./algos/map_behavior.hpp"

#include <memory>

// gm::Map&& get_map()
// {
//     return std::move(gm::Map
//     (
//         std::make_unique<gm::PopulationMetric>(),
//         gm::Map::Settings{ 12, 0.1 },
//         gm::Map::Grid{200, 200}
//     ));
// }

int main(int argc, char** argv)
{
    
    gl::engine engine { 640, 480 };
    // engine.add_behavior(std::make_unique<gm::simple_map_behavior>());
    // engine.add_behavior(std::make_unique<gm::MapBehavior>(get_map()));
    engine.add_behavior(std::make_unique<gm::MapBehavior>(gm::Map(
        gm::Map::Settings{ 12, 0.1 },
        gm::Map::Grid{200, 200}
    )));

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}