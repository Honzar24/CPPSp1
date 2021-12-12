#pragma once

#include <Object.hpp>
/**
 * @brief
 *
 *      P0---------P1
 *      |           |
 *      |     S     |
 *      |           |
 *      P3---------P2
 *  S is position from base object
 */
class Rect : public Object
{

public:
    Rect(std::string name, pos_type w, pos_type h) : Rect(name, { 0,0 }, { 0,0 }, w, h) {};
    Rect(std::string name, pos_type posx, pos_type posy, pos_type velx, pos_type vely, pos_type w, pos_type h) : Rect(name, { posx, posy }, { velx, vely }, w, h) {};
    Rect(std::string name, Vect2 pos, Vect2 vel, pos_type w, pos_type h) : Object(name, pos, vel)
    {
        this->width = w;
        this->height = h;
    };

    Vect2 getP0() const;

    Vect2 getP2() const;

    virtual ~Rect() = default;

    virtual void setPos(Vect2 pos) override;

    virtual bool contains_point(Vect2 point) const override;

    virtual Vect2 closest_point(Vect2 point) const override;

private:
    size_t width;
    size_t height;
};
