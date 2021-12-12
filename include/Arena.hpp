#pragma once
#include <cassert>

#include <algorithm>
#include <functional>
#include <ranges>
#include <vector>
#include <iostream>

#include <Object.hpp>
#include <P_types.hpp>

class Arena
{
    using TimedObject = std::tuple<size_t, size_t, Object&>;

public:

    Arena(pos_type w, pos_type h, time_type stepSize, size_t n_steps)
        : width(w), height(h), step_size(stepSize), step_count(0),
        max_step_count(n_steps)
    {}
    Arena(pos_type w, pos_type h, size_t n_steps) : Arena(w, h, 1, n_steps) {};
    Arena() : Arena(100, 100, 100) {};

    ~Arena() = default;

    void add(size_t start, size_t end, Object& o)
    {
        assert(start < end);
        objects.push_back({ start,end,o });
    }

    void step()
    {
        if (end())
        {
            return;
        }
        step_count++;
        removeEnded();
        update();
        collide();
    }

    bool end() const
    {
        return step_count >= max_step_count;
    }

private:
    const pos_type width;
    const pos_type height;
    const time_type step_size;
    size_t step_count;
    const size_t max_step_count;
    std::vector<TimedObject> objects;
    std::vector<TimedObject> ended_objects;

    const std::function<bool(TimedObject& i)> ended_L = [this](TimedObject& i) {
        auto& [start, end, o] = i;
        return end <= this->step_count;
    };
    std::function<bool(TimedObject& i)> current_L
        = [this](TimedObject& i) { return std::get<0>(i) <= this->step_count; };

    void removeEnded()
    {
        for (auto it = objects.begin(); it != objects.end();)
        {
            auto fits = std::find_if(it, objects.end(), ended_L);
            if (fits != std::end(objects))
            {
                auto fite = std::find_if_not(fits, objects.end(), ended_L);
                std::move(fits, fite, std::back_inserter(ended_objects));
                it = objects.erase(fits, fite);
            }
            it = fits;
        }
    }

    void update()
    {
        auto current_objects = objects | std::views::filter(current_L);
        for (auto& [start, end, o] : current_objects)
        {
            o.update(step_size);
        }
    }

    void collide()
    {
        auto current_objects = objects | std::views::filter([&](auto i) {
            return std::get<0>(i) <= step_count;
            });
        for (auto iti = current_objects.begin(); iti != current_objects.end();
            iti++)
        {
            auto& o1 = std::get<2>(*iti);
            auto itj = iti;
            itj++;
            for (; itj != current_objects.end(); itj++)
            {
                auto& o2 = std::get<2>(*itj);
                if (o1.collide(o2))
                {
                    std::cout << step_count << ":collision " << o1 << "\t" << o2 << std::endl;
                }
            }
        }
    }
};