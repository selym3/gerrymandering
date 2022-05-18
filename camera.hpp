#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "vec2.hpp"

namespace gm
{

// maps a value in the range [a, b] to the range [c, d]
template <typename T>
T map(const T& value, const T& a, const T& b, const T& c, const T& d)
{
    return c + (value-a) * (d - c) / (b - a);
}

// represents a window with an origin and a size. supports 
// finding equivalent points in other windows
struct window
{
    vec2d origin, size;

    window (const vec2d& origin, const vec2d& size)
        : origin { origin }, size { size }
    {
    }

    window(const vec2d& size) 
        : window(vec2d(0.0, 0.0), size)
    {
    }

    vec2d map_to(const vec2d& in_window, const window& dest_window) const
    {
        // TODO: CHange map to take in an origin and a size OR
        // remove map and implement it here
        return map(in_window, get_min(), get_max(), dest_window.get_min(), dest_window.get_max());
    }

private: 
    vec2d get_min() const { return origin; }
    vec2d get_max() const { return origin + size;} 

};

// 
struct camera
{
    window screen;
    window world;

    camera(const window& screen, const window& world)
        : screen { screen }, world { world }
    {
    }

    vec2d screen_to_world(const vec2d& in_screen) const
    {
        return screen.map_to(in_screen, world);
    }

    vec2d world_to_screen(const vec2d& in_window) const 
    {
        return world.map_to(in_window, screen);
    }

    camera& operator*=(const vec2d& zoom)
    {
        world.size *= zoom;
        return *this;
    }

    camera& operator/=(const vec2d& zoom)
    {
        world.size /= zoom;
        return *this;
    }

    camera& operator+=(const vec2d& d)
    {
        world.origin += d;
        return *this;
    }

    camera& operator-=(const vec2d& d)
    {
        world.origin -= d;
        return *this;
    }

};

}

#endif