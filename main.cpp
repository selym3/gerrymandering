#include "./graphics/engine.hpp"
#include "./algos/map_behavior.hpp"

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };
    engine.add_behavior(std::make_unique<gm::map_behavior>());

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}