#include <iostream>
#include <filesystem>

#include <Arena.hpp>

int main(int argc, char** argv) {

    Arena arena(200,200,100);
    arena.add(0,100,Object("0100",0,0,10,10));
    arena.add(0,2,Object("02",0,0,10,10));
    arena.add(5,10,Object("510",0,0,10,10));
    while(!arena.end())
    {
        arena.step();
    }
    std::cout << "Hello, world!\n";
    return 0;
}
