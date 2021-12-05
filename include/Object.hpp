#pragma once

#include <string>
#include <iostream>

#include <P_types.hpp>
#include <Vect2.hpp>

/**
 * @brief Non numeric stable representacion of model object
 *
 */
class Object
{
public:
    Object(std::string name, Vect2 pos, Vect2 vel) : name(name), position(pos), velocity(vel){};
    Object(std::string name, pos_type posx, pos_type posy, pos_type velx, pos_type vely) : Object(name, {posx, posy}, {velx, vely}){};

    ~Object() = default;

    void update(time_type delta)
    {
        Vect2 step = velocity * delta;
        position += step;
    }

    friend std::ostream &operator<<(std::ostream &out, Object &o)
    {
        out << o.name << ": pos" << o.position << " vel" << o.velocity;
        return out;
    }

private:
    std::string name;
    Vect2 position;
    Vect2 velocity;
};
