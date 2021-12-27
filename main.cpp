#include <cstdlib>
#include <cstddef>
#include <cmath>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <functional>
#include <sstream>

#include <Arena.hpp>
#include <Objects.hpp>

using namespace std::chrono_literals;
using fspath = std::filesystem::path;

//flag if parsing failed
bool parsing_fail = false;
std::stringstream REPORT;

enum class ERROR_TYPES
{
    error,
    logic,
    format,
    identifier
};

constexpr std::ostream& operator<<(std::ostream& s, ERROR_TYPES& err)
{
    switch (err)
    {
    case ERROR_TYPES::logic:
        s << "logic_error"; break;
    case ERROR_TYPES::format:
        s << "invalid_format"; break;
    case ERROR_TYPES::identifier:
        s << "identifier"; break;
    default:
        s << "error";
    }
    return s;
}

void report_append(std::string msg)
{
    REPORT << msg << std::endl;
}

void report_error(std::string errormsg, ERROR_TYPES type = ERROR_TYPES::error)
{
    report_append("ERROR");
    REPORT << type << std::endl;
    report_append(errormsg);
    parsing_fail = true;
}

struct arena_data
{
    pos_type arena_w = 0;
    pos_type arena_h = 0;
    time_type step_size = 0;
    size_t step_count = 0;
    std::map<std::string, size_t> flags;
};

struct object_data
{
    std::string name;
    pos_type posx, posy, velx, vely;
    size_t start, end;
};

inline bool seekWord(std::string word, std::fstream& file)
{
    std::string line;
    while (std::getline(file, line) && line.compare(word)) {}
    if (!file)
    {
        auto msg = "File do not cointains Keyword:" + word;
        report_error(msg, ERROR_TYPES::format);
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
    if (!file)
    {
        report_error("Not not enough parameters for arena expected 4", ERROR_TYPES::format);
        return std::nullopt;
    }
    if (ret.arena_w <= 0)
    {
        report_error("Invalid width of arena! Must be greater than 0", ERROR_TYPES::logic);
        return std::nullopt;
    }
    if (ret.arena_h <= 0)
    {
        report_error("Invalid height of arena! Must be greater than 0", ERROR_TYPES::logic);
        return std::nullopt;
    }
    if (ret.step_size <= 0)
    {
        report_error("Invalid step size of arena! Must be greater than 0", ERROR_TYPES::logic);
        return std::nullopt;
    }
    if (ret.step_count <= 0)
    {
        report_error("Invalid step count! Must be greater than 0", ERROR_TYPES::logic);
        return std::nullopt;
    }
    file >> line;
    for (;line.compare("OBJECTS") != 0;)
    {
        ret.flags[line] = 1;
        file >> line;
    }
    if (line.compare("OBJECTS") != 0)
    {
        seekWord("OBJECTS", file);
    }
    return ret;
}

struct object_data parseObject(std::fstream& file, Arena& arena)
{
    struct object_data object = {};
    file >> object.name >> object.posx >> object.posy >> object.velx >> object.vely >> object.start >> object.end;
    if (!file)
    {
        report_error("Not not enough base parameters parameters. Expected: <name> <px> <py> <vx> <vy> <start> <end>", ERROR_TYPES::format);
        return object;
    }
    if (object.posx < 0 || object.posy > arena.getWidth())
    {
        auto msg = object.name + " cannot start outside the arena! Clamping position.";
        report_error(msg, ERROR_TYPES::logic);
        object.posx = std::min(static_cast<pos_type>(0), object.posx);
        object.posy = std::min(arena.getWidth(), object.posy);
    }
    if (object.end < object.start)
    {
        auto msg = object.name + " cannot hide not shown object!";
        report_error(msg, ERROR_TYPES::logic);
        object.end = object.start;
    }
    return object;
}
bool parseSquare(std::fstream& file, Arena& arena)
{
    pos_type a;
    auto base = parseObject(file, arena);
    if (!file)
    {
        return false;
    }
    file >> a;
    if (!file)
    {
        report_error(base.name + " Not not enough parameters. Expected: <name> <px> <py> <vx> <vy> <start> <end> <a>", ERROR_TYPES::format);
        return false;
    }
    if (a < 0)
    {
        auto msg = base.name + " square line can not have negative value! Making the value unsigned.";
        report_error(msg, ERROR_TYPES::logic);
        a = std::abs(a);
    }
    arena.add(base.start, base.end, std::make_unique<Rect>(base.name, base.posx, base.posy, base.velx, base.vely, a, a));
    return true;
}
bool parseBall(std::fstream& file, Arena& arena)
{
    pos_type r;
    auto base = parseObject(file, arena);
    file >> r;
    if (!file)
    {
        report_error(base.name + " Not not enough parameters. Expected: <name> <px> <py> <vx> <vy> <start> <end> <r>", ERROR_TYPES::format);
        return false;
    }
    if (r < 0)
    {
        auto msg = base.name + " ball radius can not have negative value! Making the value unsigned.";
        report_error(msg, ERROR_TYPES::logic);
        r = std::abs(r);
    }
    arena.add(base.start, base.end, std::make_unique<Circle>(base.name, base.posx, base.posy, base.velx, base.vely, r));
    return true;
}
bool parseRectangle(std::fstream& file, Arena& arena)
{
    pos_type w, h;
    auto base = parseObject(file, arena);
    file >> w >> h;
    if (!file)
    {
        report_error(base.name + " Not not enough parameters. Expected: <name> <px> <py> <vx> <vy> <start> <end> <w> <h>", ERROR_TYPES::format);
        return false;
    }
    if (w < 0)
    {
        auto msg = base.name + " rect width can not have negative value! Making the value unsigned.";
        report_error(msg, ERROR_TYPES::logic);
        w = std::abs(w);
    }
    if (h < 0)
    {
        auto msg = base.name + " rect height can not have negative value! Making the value unsigned.";
        report_error(msg, ERROR_TYPES::logic);
        h = std::abs(h);
    }
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
    auto msg = "Unknow type: " + type + " is not in [square,ball,rectangle]";
    report_error(msg, ERROR_TYPES::identifier);
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
    if (parser == nullptr)
    {
        return false;
    }
    if (!file)
    {
        report_error("Expected count of " + type + "got none!", ERROR_TYPES::format);
        return false;
    }
    file >> count;
    for (size_t i = 0; i < count; i++)
    {
        if (!parser(file, arena))
        {
            report_error("Expected " + std::to_string(count) + " " + type + " got " + std::to_string(i), ERROR_TYPES::logic);
            return false;
        }
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
        return;
    }
    out << REPORT.str() << std::endl;
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
        return EXIT_FAILURE;
    }
    report_append("REPORT");

    ///parsing

    auto dataBox = parseArena(in);
    if (!dataBox.has_value())
    {
        export_report(output);
        return EXIT_FAILURE;
    }
    auto data = dataBox.value();
    Arena arena(data.arena_w, data.arena_h, data.step_size, data.step_count);
    while (parseSection(in, arena));;

    if (parsing_fail)
    {
        export_report(output);
        return EXIT_FAILURE;
    }

    ///simulation

    std::chrono::steady_clock timer;
    std::function<bool()> end_sim = [&arena] {return !arena.end();};
    if (data.flags["end_after_collision"] == 1)
    {
        end_sim = [&arena] {return !arena.end() && arena.numberOfCollisoin() < 1;};
    }

    auto start = timer.now();
    while (end_sim())
    {
        arena.step();
    }
    auto end = timer.now();


    ///Reporting

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    report_append("OK");
    report_append(std::to_string(arena.getStep()));
    if (arena.getStep() >= data.step_count)
    {
        report_append("timeout");
    } else if (arena.numberOfCollisoin() == 1)
    {
        report_append("collision");
    }
    std::string dur_str;
    dur_str += std::to_string(duration.count());
    dur_str += " ms";
    report_append(dur_str);
    report_append("COLLISIONS");
    arena.report_collisions(REPORT);
    if (data.flags["report_distance"] == 1)
    {
        report_append("DISTANCES");
        arena.report_distances(REPORT);
    }
    export_report(output);
    return EXIT_SUCCESS;
}
