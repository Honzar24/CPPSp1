#pragma once

#include <Object.hpp>

class Circle : public Object
{
public:
    Circle(std::string name, pos_type r) : Circle(name, { 0,0 }, { 0,0 }, r) {};
    Circle(std::string name, pos_type posx, pos_type posy, pos_type velx, pos_type vely, pos_type r) : Circle(name, { posx, posy }, { velx, vely }, r) {};
    Circle(std::string name, Vect2 pos, Vect2 vel, pos_type r) : Object(name, pos, vel), r(r) {};

    virtual ~Circle() {};

    virtual bool contains_point(Vect2 point) const override;

    virtual Vect2 closest_point(Vect2 point) const override;


private:
    const pos_type r;
};


