#pragma once

#include <P_types.hpp>

class Vect2
{
public:
    pos_type x;
    pos_type y;

    Vect2(pos_type x, pos_type y) : x(x), y(y){};
    Vect2() : Vect2(0, 0){};

    ~Vect2() = default;

    Vect2 &operator+=(Vect2 &ov)
    {
        x += ov.x;
        y += ov.y;
        return *this;
    }
    Vect2 &operator*(pos_type s)
    {
        x *= s;
        y *= s;
        return *this;
    }
};

std::ostream &operator<<(std::ostream &out, Vect2 &v)
{
    out << "[" << v.x << "," << v.y << "]";
    return out;
}