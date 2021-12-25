#pragma once

#include <P_types.hpp>
#include <tuple>
#include <cmath>

class Vect2
{
public:
    pos_type x;
    pos_type y;

    Vect2(const pos_type x, const pos_type y) : x(x), y(y) {};
    Vect2() : Vect2(0, 0) {};

    ~Vect2() = default;

    inline Vect2& operator+(const Vect2& ov)
    {
        x += ov.x;
        y += ov.y;
        return *this;
    }
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
    inline bool operator==(Vect2& o) const
    {
        return std::tie(x, y) == std::tie(o.x, o.y);
    }

    inline pos_type lenght() const
    {
        return std::abs(std::sqrt(x * x + y * y));
    }

    Vect2 norm()
    {
        pos_type l = lenght();
        if (lenght() == 0)
        {
            return *this;
        }
        x /= l;
        y /= l;
        return *this;
    }

    inline friend std::ostream& operator<<(std::ostream& out, Vect2& v)
    {
        out << "[" << v.x << "," << v.y << "]";
        return out;
    }
};

