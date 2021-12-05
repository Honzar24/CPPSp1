#pragma once

#include <cstddef>

class Arena
{
    using pos_t = size_t;

public:
    enum class Flags
    {
        NONE,
        END_AFTER_COLLISION,
        REPORT_DISTANCE
    };

    Arena(pos_t w, pos_t h):width(w),height(h){}

    private:
    pos_t width = 100;
    pos_t height = 100;
    double stepsize = 0.1;

};