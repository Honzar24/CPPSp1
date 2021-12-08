#include <Object.hpp>

Object::~Object()
{

}

void Object::update(time_type delta)
{
    Vect2 step = velocity * delta;
    position += step;
}

void Object::setPos(pos_type px, pos_type py)
{
    setPos({ px,py });
}

void Object::setPos(Vect2 pos)
{
    position = pos;
}

bool Object::contains_point(pos_type px, pos_type py)
{
    return contains_point({ px,py });
}

Vect2 Object::closest_point(pos_type px, pos_type py)
{
    return closest_point({ px,py });
}

std::ostream& operator<<(std::ostream& out, Object& o)
{
    out << o.name << ": pos" << o.position << " vel" << o.velocity;
    return out;
}

bool Object::collide(Object& o)
{
    auto p1 = this->closest_point(o.position);
    auto p2 = o.closest_point(this->position);
    return contains_point(p2) || o.contains_point(p1);
}