#include <cmath>

#include <Rect.hpp>

Vect2 Rect::getP0() const
{
    return { position.x - static_cast<int>(height) / 2,position.y - static_cast<int>(width) / 2 };
}
Vect2 Rect::getP2() const
{
    return { position.x + static_cast<int>(height) / 2,position.y + static_cast<int>(width) / 2 };
}

bool Rect::contains_point(Vect2 point) const
{
    auto p0 = getP0();
    auto p2 = getP2();
    return p0.x <= point.x && point.x <= p2.x && p0.y <= point.y && point.y <= p2.y;
}
Vect2 Rect::closest_point(Vect2 point) const
{
    pos_type x, y;
    auto p0 = getP0();
    auto p2 = getP2();
    x = std::max(p0.x, std::min(p2.x, point.x));
    y = std::max(p0.y, std::min(p2.y, point.y));
    return { x,y };
}