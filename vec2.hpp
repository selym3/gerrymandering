#ifndef __VEC2_HPP__
#define __VEC2_HPP__

#include <ostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace gm
{

template <typename T>
struct vec2
{
    //////////
    // DATA //
    //////////

    T x, y;

    //////////////////
    // CONSTRUCTORS //
    //////////////////

    vec2(const T& x, const T& y) : 
        x {x}, y {y}
    {
    }

    vec2(const T& xy) :
        vec2(xy, xy)
    {
    }

    vec2() :
        vec2(T{0})
    {
    }

    template <typename U>
    static vec2 from(const U& v) 
    {
        return vec2( static_cast<T>(v.x), static_cast<T>(v.y) ); 
    }

    template <typename U>
    sf::Vector2<U> to() const
    {
        return sf::Vector2<U>{ static_cast<U>(x), static_cast<U>(y) };
    }

    /////////////////
    // VECTOR MATH //
    /////////////////

    T dot(const vec2& rhs)
    {
        return x * rhs.x + y * rhs.y;
    }

    T length() const
    {
        return std::hypot(x, y);
    }

    ////////////////
    // ARITHMETIC //
    ////////////////

    vec2& operator+=(const vec2& rhs)
    {
        x += rhs.x, y += rhs.y;
        return *this;
    }

    friend vec2 operator+(vec2 lhs, const vec2& rhs)
    {
        return lhs += rhs;
    }

    vec2& operator-=(const vec2& rhs)
    {
        x -= rhs.x, y -= rhs.y;
        return *this;
    }

    friend vec2 operator-(vec2 lhs, const vec2& rhs)
    {
        return lhs -= rhs;
    }

    vec2& operator*=(const vec2& rhs)
    {
        x *= rhs.x, y *= rhs.y;
        return *this;
    }

    friend vec2 operator*(vec2 lhs, const vec2& rhs)
    {
        return lhs *= rhs;
    }

    vec2& operator/=(const vec2& rhs)
    {
        x /= rhs.x, y /= rhs.y;
        return *this;
    }

    friend vec2 operator/(vec2 lhs, const vec2& rhs)
    {
        return lhs /= rhs;
    }

    ///////////////
    // OPERATORS //
    ///////////////

    friend std::ostream& operator<<(std::ostream& os, const vec2& rhs)
    {
        return os << "( " << rhs.x << " , " << rhs.y << " )";
    }

};

using vec2d = vec2<double>;
using vec2i = vec2<int>;

}

#endif