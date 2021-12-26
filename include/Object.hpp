#pragma once

#include <string>
#include <iostream>

#include <P_types.hpp>
#include <Vect2.hpp>

/**
 * @brief Abstact non numeric stable representacion of model object+
 */
class Object
{
public:
    Object(std::string name) :Object(name, 0, 0, 0, 0) {};

    Object(std::string name, pos_type posx, pos_type posy, pos_type velx, pos_type vely) : Object(name, { posx, posy }, { velx, vely }) {};

    Object(std::string name, Vect2 pos, Vect2 vel) : name(name), velocity(vel), position(pos) {};

    virtual ~Object();

    friend std::ostream& operator<<(std::ostream& out, Object& o);

    void setPos(pos_type px, pos_type py);

    void setPos(Vect2& pos);

    Vect2 getPos();

    void setVel(pos_type vx, pos_type vy);

    void setVel(Vect2& vel);

    Vect2 getVel();

    void setName(std::string& nname);

    std::string getName();

    void update(time_type delta);

    bool contains_point(pos_type px, pos_type py) const;

    Vect2 closest_point(pos_type px, pos_type py) const;

    bool collide(Object& o) const;

    //pure virtual

    virtual bool contains_point(Vect2 point) const = 0;

    virtual Vect2 closest_point(Vect2 point) const = 0;


private:
    std::string name;
    Vect2 velocity;
protected:
    /// Refers to position of center of object
    Vect2 position;
};
