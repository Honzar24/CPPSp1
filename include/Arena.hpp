#pragma once
#include <cassert>

#include <vector>
#include <ranges>
#include <functional>
#include <algorithm>

#include <P_types.hpp>
#include <Object.hpp>

class Arena
{
    using  TimedObject = std::tuple<size_t,size_t,Object>;
public:
    enum class Flags
    {
        NONE,
        END_AFTER_COLLISION,
        REPORT_DISTANCE
    };

    Arena(pos_type w, pos_type h,time_type stepSize,size_t n_steps):width(w),height(h),step_size(stepSize),step_count(0),max_step_count(n_steps){}
    Arena(pos_type w, pos_type h,size_t n_steps):Arena(w,h,1,n_steps){};
    Arena():Arena(100,100,100){};

    ~Arena() = default;

    void add(size_t start,size_t end, Object& o)
    {
        add(start,end,std::move(o));
    }

    void add(size_t start,size_t end, Object&& o)
    {
        assert(start < end);
        objects.push_back({start,end,o});
    }

    void step()
    {
        if(end())
        {
            return;
        }
        step_count++;
        removeEnded();
        update();
        

    }

    bool end()
    {
        return step_count >= max_step_count;
    }

    private:
    pos_type width;
    pos_type height;
    time_type step_size;
    size_t step_count;
    size_t max_step_count;
    std::vector<TimedObject> objects;
    std::vector<TimedObject> endeds;

    void removeEnded()
    {
        auto ended = [&](auto i) -> bool{ 
            auto&[start,end,o] = i;
            return end <= step_count;};
        for(auto it = objects.begin(); it != objects.end();)
        {
            auto fits = std::find_if(it,objects.end(),ended);
            if(fits != std::end(objects))
            {
                auto fite = std::find_if_not(fits,objects.end(),ended);
                std::move(fits,fite,std::back_inserter(endeds));
                it  = objects.erase(fits,fite);
            }
            it = fits;
        }
    }

    void update()
    {
        auto current_objects = objects | std::views::filter([&](auto i) {return std::get<0>(i) <= step_count;});
        for(auto&[start,end,o]:current_objects)
        {
            o.update(step_size);
        }
    }

};