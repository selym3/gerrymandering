#include "./graphics/engine.hpp"
#include "./algos/map_behavior.hpp"
#include "./algos/map2.hpp"

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };
    // engine.add_behavior(std::make_unique<gm::map_behavior>());
    engine.add_behavior(std::make_unique<gm::Map2_behavior>());

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}