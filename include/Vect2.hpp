#pragma once

#include <P_types.hpp>
#include <tuple>

class Vect2
{
public:
    pos_type x;
    pos_type y;

    Vect2(pos_type x, pos_type y) : x(x), y(y) {};
    Vect2() : Vect2(0, 0) {};

    ~Vect2() = default;

    inline Vect2& operator+=(Vect2& ov)
    {
        x += ov.x;
        y += ov.y;
        return *this;
    }
    inline Vect2& operator*(pos_type s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    inline bool operator==(Vect2 o)
    {
        return std::tie(x, y) == std::tie(o.x, o.y);
    }
};

inline std::ostream& operator<<(std::ostream& out, Vect2& v)
{
    out << "[" << v.x << "," << v.y << "]";
    return out;
}