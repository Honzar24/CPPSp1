#include <cstdlib>
#include <cstddef>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include <sstream>

#include <Arena.hpp>
#include <Objects.hpp>

using namespace std::chrono_literals;
using fspath = std::filesystem::path;

constexpr size_t FLAGS_COUNT = 2;

std::stringstream REPORT;

struct arena_data
{
    pos_type arena_w;
    pos_type arena_h;
    time_type step_size;
    size_t step_count;
    std::vector<std::string> flags;
};

struct object_data
{
    std::string name;
    pos_type posx, posy, velx, vely;
    size_t start, end;
};

bool seekWord(std::string word, std::fstream& file)
{
    std::string line;
    while (std::getline(file, line) && line.compare(word)) {}
    if (!file)
    {
        REPORT << "ERROR" << std::endl << "invalid_format" << std::endl << "File do not cointains Keyword:" << word << std::endl;
        return false;
    }
    return true;
}
std::optional<struct arena_data> parseArena(std::fstream& file)
{
    std::string line;
    arena_data ret;
    if (!seekWord("ARENA", file))
    {
        return std::nullopt;
    }
    file >> ret.arena_w >> ret.arena_h >> ret.step_size >> ret.step_count;
    file >> line;
    for (;line.compare("OBJECTS") != 0;)
    {
        ret.flags.push_back(line);
        file >> line;
    }
    if (line.compare("OBJECTS") != 0)
    {
        seekWord("OBJECTS", file);
    }
    return ret;
}

struct object_data parseObject(std::fstream& file)
{
    struct object_data object;
    file >> object.name >> object.posx >> object.posy >> object.velx >> object.vely >> object.start >> object.end;
    return object;
}
bool parseSquare(std::fstream& file, Arena& arena)
{
    pos_type a;
    auto base = parseObject(file);
    file >> a;
    arena.add(base.start, base.end, std::make_unique<Rect>(base.name, base.posx, base.posy, base.velx, base.vely, a, a));
    return true;
}
bool parseBall(std::fstream& file, Arena& arena)
{
    pos_type r;
    auto base = parseObject(file);
    file >> r;
    arena.add(base.start, base.end, std::make_unique<Circle>(base.name, base.posx, base.posy, base.velx, base.vely, r));
    return true;
}
bool parseRectangle(std::fstream& file, Arena& arena)
{
    pos_type w, h;
    auto base = parseObject(file);
    file >> w >> h;
    arena.add(base.start, base.end, std::make_unique<Rect>(base.name, base.posx, base.posy, base.velx, base.vely, w, h));
    return true;
}

std::function<bool(std::fstream& file, Arena& arena)> getParser(std::string& type)
{
    if (type.compare("square") == 0)
    {
        return parseSquare;
    }
    if (type.compare("ball") == 0)
    {
        return parseBall;
    }
    if (type.compare("rectangle") == 0)
    {
        return parseRectangle;
    }
    REPORT << "ERROR" << std::endl << "identifier" << std::endl << "Unknow type: " << type << " is not in [square,ball,rectangle]" << std::endl;
    return nullptr;
}

bool parseSection(std::fstream& file, Arena& arena)
{
    std::function<bool(std::fstream& file, Arena& arena)> parser;
    size_t count;
    std::string type;
    file >> type;
    if (!file)
    {
        return false;
    }
    parser = getParser(type);
    file >> count;
    for (size_t i = 0; i < count; i++)
    {
        parser(file, arena);
    }
    return true;
}

void export_report(std::string& outFileName)
{
    std::fstream out;
    if (outFileName.compare("null") == 0)
    {
        return;
    }
    if (outFileName.compare("cout") == 0)
    {
        std::cout << REPORT.str() << std::endl;
        return;
    }
    fspath output(outFileName);
    out.open(output.make_preferred().string(), std::ios::trunc | std::ios::out);
    if (!out.is_open())
    {
        std::cerr << "Can not open output file " << output.make_preferred().string() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void report_append(std::string msg)
{
    REPORT << msg << std::endl;
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage " << argv[0] << " <input file> <cout/null/uoput file>" << std::endl;
        return EXIT_SUCCESS;
    }
    fspath input = argv[1];
    std::string output = argv[2];
    std::fstream in(input.make_preferred().string());
    if (!in.is_open())
    {
        std::cerr << "Can not open input file " << input.make_preferred().string() << std::endl;
        exit(EXIT_FAILURE);
    }
    report_append("REPORT");


    auto dataBox = parseArena(in);
    if (!dataBox.has_value())
    {
        export_report(output);
        exit(EXIT_FAILURE);
    }
    auto data = dataBox.value();
    Arena arena(data.arena_w, data.arena_h, data.step_size, data.step_count);
    while (parseSection(in, arena));;

    //end after col

    std::chrono::steady_clock timer;
    auto start = timer.now();
    while (!arena.end())
    {
        arena.step();
    }
    auto end = timer.now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    report_append("OK");
    report_append(std::string(arena.getStep() + ""));
    if(arena.getStep() >= data.step_count)
    {
        report_append("timeout");
    }
    if(false) 
    {
        report_append("collision");
    }
    report_append(duration.count()+" ms");
    report_append("COLLISIONS");
    //arena.report_collisions(outs);
    if(false)
    {
        report_append("DISTANCES");
        //arena.report_distances(outs);
    }
    export_report(output);
    return EXIT_SUCCESS;
}
