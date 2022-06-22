#include "./graphics/engine.hpp"

#include "./algos/map.hpp"
#include "./algos/map_behavior.hpp"

#include <memory>

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };
    // engine.add_behavior(std::make_unique<gm::simple_map_behavior>());
    engine.add_behavior(std::make_unique<gm::MapBehavior>(10));

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}