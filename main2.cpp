#include "./graphics/engine.hpp"

int main(int argc, char** argv)
{
    gl::engine engine { 640, 480 };

    while (engine.is_running())
    {
        engine.execute();
    }

    return 0;
}