#include <cassert>

#include <Arena.hpp>

std::string report(size_t step, Object& o1, Object& o2)
{
    std::stringstream report;
    report << o1.getName()<< " "<< o2.getName() << " " << step <<  " ";
    Vect2 pos = o1.getPos();
    report << pos.x << " " <<  pos.y <<" ";
    pos = o2.getPos();
    report << pos.x << " " <<  pos.y;
    return report.str();
}

void Arena::removeEnded()
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

void Arena::update()
{
    auto current_objects = objects | std::views::filter(current_L);
    for (auto& [start, end, o , dist] : current_objects)
    {
        Vect2 pos = o->getPos();
        if (pos.x >= width)
        {
            Vect2 pos = o->getPos();
            if (pos.x >= width)
            {
                Vect2 vel = o->getVel();
                vel.x *= -1;
                o->setVel(vel);
            }
            if (pos.y >= height)
            {
                Vect2 vel = o->getVel();
                vel.y *= -1;
                o->setVel(vel);
            }
        }
        o->update(step_size);
        Vect2 rpos = o->getPos();
        auto distance = (rpos + (pos * -1)).lenght();
        assert(distance >= 0);
        dist += distance;
    }
}

void Arena::collide()
{
    auto current_objects = objects | std::views::filter(current_L);
    for (auto iti = current_objects.begin(); iti != current_objects.end();
        iti++)
    {
        auto& o1 = std::get<2>(*iti);
        auto itj = iti;
        itj++;
        for (; itj != current_objects.end(); itj++)
        {
            auto& o2 = std::get<2>(*itj);
            if (o1->collide(*o2))
            {
                collision_report.push_back(report(step_count + 1,*o1,*o2));
            }
        }
    }
}

void Arena::report_collisions(std::stringstream& s)
{
    s << collision_report.size() << std::endl;
    for (std::string& c : collision_report)
    {
        s << c << std::endl;
    }
}

void Arena::report_distances(std::stringstream& s)
{
    s << objects.size() + ended_objects.size() << std::endl;
    for(auto& i : objects)
    {
        auto&[start,end,o,dist] = i;
        s << o->getName() << " " << dist << std::endl;
    }
    for(auto& i : ended_objects)
    {
        auto&[start,end,o,dist] = i;
        s << o->getName() << " " << dist << std::endl;
    }
}