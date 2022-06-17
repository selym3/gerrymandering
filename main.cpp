#include "./graphics/engine.hpp"
// #include "./algos/simple_map_behavior.hpp"
// #include "./algos/_map.hpp"

#include "./algos/map.hpp"
#include "./algos/map_behavior.hpp"

#include <memory>


void configure(gm::Map& map)
{

}

std::shared_ptr<gm::Map> make_map()
{
    std::shared_ptr<gm::Map> map = std::make_shared<gm::Map>();
    configure(*map);
    return map;
}

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };
    // engine.add_behavior(std::make_unique<gm::simple_map_behavior>());
    engine.add_behavior(std::make_unique<gm::MapBehavior>(4));

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}