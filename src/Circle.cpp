

#include <Circle.hpp>

bool Circle::contains_point(Vect2 point) const
{
    return std::abs(point.x - position.x) <= r && std::abs(point.y - position.y) <= r;
}

Vect2 Circle::closest_point(Vect2 point) const
{
    auto secna = (point + position);
    auto smer = secna.norm();
    return smer * r * (-1) + position;
}