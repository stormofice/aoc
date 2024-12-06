#include "Day6.h"

#include "../lib.h"
#include <utility>

bool debug = false;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

struct tuple_hash {
    template <class T1, class T2, class T3>
    std::size_t operator()(const std::tuple<T1, T2, T3> &tuple) const {
        auto hash1 = std::hash<T1>{}(std::get<0>(tuple));
        auto hash2 = std::hash<T2>{}(std::get<1>(tuple));
        auto hash3 = std::hash<T3>{}(std::get<2>(tuple));


        std::size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};

enum class Direction {
    North,
    South,
    West,
    East,
};
using game_map = std::vector<std::vector<char>>;
struct Game {
    int playerx;
    int playery;
    Direction dir;
    game_map map;
    int width;
    int height;
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::unordered_set<std::tuple<int, int, Direction>, tuple_hash> pvis;
};

std::pair<int, int> dir_to_move(Direction &d) {
    switch (d) {
    case Direction::North:
        return {0, -1};
    case Direction::South:
        return {0, 1};
    case Direction::West:
        return {-1, 0};
    case Direction::East:
        return {1, 0};
    }
    assert(false);
    std::abort();
    return {-1000, -1000};
}

Direction next_dir(Direction &d) {
    switch (d) {
    case Direction::North:
        return Direction::East;
    case Direction::South:
        return Direction::West;
    case Direction::West:
        return Direction::North;
    case Direction::East:
        return Direction::South;
    }
    assert(false);
    std::abort();
    return Direction::North;
}

void visualize(Game &g) {
    if (!debug) {
        return;
    }

    for (const auto &line : g.map) {
        for (const auto &item : line) {
            std::print("{}", item);
        }
        std::println();
    }
}

void simulate(Game &g) {
    while (true) {
        g.visited.insert(std::make_pair(g.playerx, g.playery));

        auto move = dir_to_move(g.dir);
        auto next = std::pair<int, int>({g.playerx + move.first, g.playery + move.second});

        if (next.first >= g.width || next.second >= g.height || next.first < 0 || next.second < 0) {
            std::println("Leaving map, breaking");
            break;
        }

        auto nchar = g.map[next.second][next.first];
        // std::println("Next char: {}", nchar);



        switch (nchar) {
        case '^':
            assert(false);
            break;
        case '.':
            g.map[g.playery][g.playerx] = '.';
            g.playerx = next.first;
            g.playery = next.second;
            g.map[g.playery][g.playerx] = '^';
            break;
        case '#':
            g.dir = next_dir(g.dir);
            break;
        }

        visualize(g);
        //  std::cin.get();
    }
}

enum class SimulationResult {
    HeGone,
    Loop,
};

SimulationResult obstruction_simulate(Game &g) {
    while (true) {
        auto is = g.pvis.emplace(g.playerx, g.playery, g.dir);
        if (is.second == false) {
            return SimulationResult::Loop;
        }

        auto move = dir_to_move(g.dir);
        auto next = std::pair<int, int>({g.playerx + move.first, g.playery + move.second});

        if (next.first >= g.width || next.second >= g.height || next.first < 0 || next.second < 0) {
            // std::println("Leaving map, he gone");
            return SimulationResult::HeGone;
        }


        auto nchar = g.map[next.second][next.first];
        if (debug)
            std::println("Next char: {}", nchar);

        switch (nchar) {
        case '^':
            assert(false);
            break;
        case '.':
            g.map[g.playery][g.playerx] = '.';
            g.playerx = next.first;
            g.playery = next.second;
            g.map[g.playery][g.playerx] = '^';
            break;
        case '#':
            g.dir = next_dir(g.dir);
            break;
        }
        if (debug) {
            visualize(g);
            std::cin.get();
        }
    }
}

void Day6::run() {
    constexpr auto file_path = "./inputs/day6.input";
    std::ifstream infile(file_path);

    char c;
    auto idx = 0;

    Game g{
        .playerx = 0,
        .playery = 0,
        .dir = Direction::North,
        .map = game_map{},
        .width = 0,
        .height = 0,
        .visited = std::unordered_set<std::pair<int, int>, pair_hash>{},
        .pvis = std::unordered_set<std::tuple<int, int, Direction>, tuple_hash>{},

    };
    g.map.emplace_back(std::vector<char>());

    auto lidx = 0;

    while (infile >> std::noskipws >> c) {
        if (c == '\n') {
            idx++;
            lidx = 0;
            g.map.emplace_back(std::vector<char>());
            continue;
        }
        g.map[idx].push_back(c);

        if (c == '^') {
            g.playerx = lidx;
            g.playery = idx;
        }
        lidx++;
    }
    g.height = g.map.size();
    g.width = g.map[0].size();

    std::println("Input:");
    visualize(g);
    auto g1t = g;
    simulate(g1t);
    // std::println("Visited {} unique places", g.visited.size());

    std::println("Starting obstruction search!");

    auto loop_count = 0;
    auto sims = 0;

    for (const auto & [ x, y ] : g1t.visited) {
        char c = g.map[y][x];
        switch (c) {
        case '^':
        case '#':
            continue;
        case '.': {

            // Game g2 = g;
            auto px = g.playerx;
            auto py = g.playery;
            auto pd = g.dir;

            g.map[y][x] = '#';

            auto res = obstruction_simulate(g);
            if (res == SimulationResult::Loop) {
                loop_count++;
                // std::println("Sim[{}][{}] -> Loop", y, x);
            } else {
                // std::println("Sim[{}][{}] -> Gone", y, x);
            }
            sims++;

            g.map[y][x] = '.';
            g.map[g.playery][g.playerx] = '.';
            g.map[py][px] = '^';
            g.pvis.clear();
            g.playerx = px;
            g.playery = py;
            g.dir = pd;
        }
        }
    }

    std::println("Possible loops: {}, Total simulations: {}", loop_count, sims);
}
