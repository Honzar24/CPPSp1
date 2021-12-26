#include <Object.hpp>

Object::~Object()
{

}



void Object::setPos(pos_type px, pos_type py)
{
    Vect2 p = { px,py };
    setPos(p);
}

void Object::setPos(Vect2& pos)
{
    position = pos;
}

Vect2 Object::getPos(){
    return position;
}

void Object::setVel(pos_type vx,pos_type vy)
{
    Vect2 v = {vx,vy};
    setVel(v);
}

void Object::setVel(Vect2& vel)
{
    velocity = vel;
}

Vect2 Object::getVel()
{
    return velocity;
}

void Object::setName(std::string& nname){
    name = nname;
}

std::string Object::getName(){
    return name;
}

void Object::update(time_type delta)
{
    Vect2 step = velocity * delta;
    position += step;
}

bool Object::contains_point(pos_type px, pos_type py) const
{
    return contains_point({ px,py });
}

Vect2 Object::closest_point(pos_type px, pos_type py) const
{
    return closest_point({ px,py });
}

std::ostream& operator<<(std::ostream& out, Object& o)
{
    out << o.name << ": pos" << o.position << " vel" << o.velocity;
    return out;
}

bool Object::collide(Object& o) const
{
    if (this->position == o.position)
    {
        return true;
    }
    return contains_point(o.closest_point(this->position)) || o.contains_point(closest_point(o.position));
}