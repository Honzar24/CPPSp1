#pragma once
#include <cassert>

#include <algorithm>
#include <functional>
#include <ranges>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include <Object.hpp>
#include <P_types.hpp>

class Arena
{
    using TimedObject = std::tuple<size_t, size_t, std::unique_ptr<Object>, pos_type>;

public:

    Arena(pos_type w, pos_type h, time_type stepSize, size_t n_steps)
        : width(w), height(h), step_size(stepSize), step_count(0),
        max_step_count(n_steps)
    {}
    Arena(pos_type w, pos_type h, size_t n_steps) : Arena(w, h, 1, n_steps) {};
    Arena() : Arena(100, 100, 100) {};

    ~Arena() = default;

    void add(size_t start, size_t end, std::unique_ptr<Object> ptr)
    {
        assert(start <= end);
        objects.emplace_back(start, end, std::move(ptr), 0);
    }

    void step()
    {
        if (end())
        {
            return;
        }
        removeEnded();
        update();
        collide();
        if (step_count != max_step_count)
        {
            step_count++;
        }
    }

    bool end() const
    {
        return step_count >= max_step_count;
    }

    size_t getStep()const
    {
        return step_count;
    }

    size_t numberOfCollisoin()
    {
        return collision_report.size();
    }

    pos_type getWidth()
    {
        return width;
    }

    pos_type heigtht()
    {
        return height;
    }

    void report_collisions(std::stringstream& s);

    void report_distances(std::stringstream& s);


private:
    const pos_type width;
    const pos_type height;
    const time_type step_size;
    size_t step_count;
    const size_t max_step_count;
    std::vector<TimedObject> objects;
    std::vector<TimedObject> ended_objects;
    std::vector<std::string> collision_report;

    const std::function<bool(TimedObject& i)> ended_L = [this](TimedObject& i) {
        auto& [start, end, o, dist] = i;
        return end <= this->step_count;
    };
    std::function<bool(TimedObject& i)> current_L = [this](TimedObject& i) {
        auto& [start, end, o, dist] = i;
        return start <= this->step_count;
    };

    void removeEnded();

    void update();

    void collide();
};