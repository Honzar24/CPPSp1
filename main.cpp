#include <iostream>
#include <filesystem>

#include <Arena.hpp>
#include <Objects.hpp>

enum class Flags
{
    NONE,
    END_AFTER_COLLISION,
    REPORT_DISTANCE
};

int main(int argc, char** argv)
{

    Arena arena(200, 200, 40);
    auto r1 = Rect(std::string("r1"), 0, 0, 10, 5, 10, 10);
    auto r2 = Rect(std::string("r2"), 0, 0, 10, 5, 10, 10);
    auto c1 = Circle(std::string("c1"), 0, 0, 10, 10, 0);

    arena.add(0, 100, r1);
    arena.add(0, 10, r2);
    arena.add(0, 100, c1);

    while (!arena.end())
    {
        arena.step();
    }
    return 0;
}
