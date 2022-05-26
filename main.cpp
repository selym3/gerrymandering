#include "./graphics/engine.hpp"
#include "./algos/simple_map_behavior.hpp"
// #include "./algos/map.hpp"

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };
    engine.add_behavior(std::make_unique<gm::simple_map_behavior>());
    // engine.add_behavior(std::make_unique<gm::Map_behavior>());

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}