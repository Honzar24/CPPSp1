#include <iostream>
#include <filesystem>

#include <Arena.hpp>
#include <Rect.hpp>

int main(int argc, char** argv)
{

    Arena arena(200, 200, 40);
    auto r1 = Rect(std::string("r1"),0,0,10,10,10,10);
    auto r2 = Rect(std::string("r2"),10,-10.5,10,10,10,10);
    auto b = r1.collide(r2);
    auto bi = r2.collide(r1);

    Object& o1 = r1;
    Object& o2 = r2;

    auto bo = o1.collide(o2);


    arena.add(0, 100, r1);
    arena.add(0, 20, r2);
    while (!arena.end())
    {
        arena.step();
    }
    return 0;
}
