#include "Day8.h"

#include "../lib.h"

static_assert(sizeof(std::size_t) == 8, "std::size_t is not 64 bits");
static_assert(sizeof(int) == 4, "int is not 32 bits");

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};

// jaja
int width = 0, height = 0;

using point = std::pair<int, int>;
using antenna_set = std::unordered_set<point, uwu_hash>;

point distance(const point &first, const point &second) {
    return std::make_pair<int, int>(first.first - second.first, first.second - second.second);
}

point inverse(const point &v) {
    return std::make_pair<int, int>(-v.first, -v.second);
}

bool is_valid_point(const point &p) {
    return p.first >= 0 && p.second >= 0 && p.first <= width && p.second <= height;
}

point add(const point &p, const point &s) {
    return std::make_pair<int, int>(p.first + s.first, p.second + s.second);
}

void find_antinode_pt1(antenna_set &as, std::unordered_set<point, uwu_hash> &antinodes) {
    for (const auto &outer : as) {
        for (const auto &inner : as) {
            if (outer == inner) {
                continue;
            }

            auto dist = distance(outer, inner);
            auto inv = inverse(dist);
            auto od = std::make_pair<int, int>(outer.first + dist.first, outer.second + dist.second);



            auto id = std::make_pair<int, int>(inner.first + inv.first, inner.second + inv.second);

            antinodes.emplace(od);
            antinodes.emplace(id);
        }
    }
}


void visualize(std::unordered_set<point, uwu_hash> &overlaps) {
    for (auto y = 0; y <= height; y++) {
        for (auto x = 0; x <= width; x++) {
            if (overlaps.contains(std::make_pair(x, y))) {
                std::print("#");
            } else {
                std::print(".");
            }
        }
        std::println();
    }
    std::println("--------");
}


void find_antinode(antenna_set &as, std::unordered_set<point, uwu_hash> &antinodes) {
    if (as.size() == 1)
        return;

    for (const auto &outer : as) {
        for (const auto &inner : as) {
            if (outer == inner) {
                continue;
            }

            // std::println("{},{}  {},{}", outer.first, outer.second, inner.first, inner.second);

            // cheap fix
            antinodes.emplace(outer);
            antinodes.emplace(inner);


            // visualize(antinodes);
            auto dist = distance(outer, inner);

            auto od = outer;
            while (true) {
                od = add(od, dist);

                auto v = is_valid_point(od);
                if (v) {
                    antinodes.emplace(od);
                } else {
                    break;
                }
            }

            auto inv = inverse(dist);
            auto id = inner;
            while (true) {
                id = add(id, inv);

                auto v = is_valid_point(id);
                if (v) {
                    antinodes.emplace(id);
                } else {
                    break;
                }
            }

            // visualize(antinodes);
        }
    }
}

void find_antinodes(std::unordered_map<char, antenna_set> &antennas, std::unordered_set<point, uwu_hash> &antinodes) {
    for (auto &item : antennas) {
        find_antinode(item.second, antinodes);
    }
}

void Day8::run() {
    constexpr auto file_path = "./inputs/day8.input";
    std::ifstream infile(file_path);

    std::unordered_map<char, antenna_set> antennas;

    char c;
    int y = 0, x = 0;

    while (infile >> std::noskipws >> c) {
        if (c == '\n') {
            width = (x - 1);
            x = 0;
            y++;
            continue;
        }

        if (c != '.') {
            auto &aset = antennas.emplace(c, antenna_set{}).first->second;
            aset.emplace(x, y);
        }

        x++;
    }
    height = y;

    std::println("Width: {}, Height: {}", width, height);


    for (const auto & [ c, s ] : antennas) {
        std::print("{}: ", c);
        for (const auto & [ x, y ] : s) {
            std::print("({}, {}), ", x, y);
        }
        std::println();
    }

    std::unordered_set<point, uwu_hash> overlaps{};
    find_antinodes(antennas, overlaps);

    /*
    auto filtered = overlaps | std::views::filter([&width, &height](const point &p) {
                        return p.first >= 0 && p.first <= width && p.second >= 0 && p.second <= height;
                    });
    */
    /*
    auto antinodes = 0;
    for (const auto &p : filtered) {
        std::print("({}, {}), ", p.first, p.second);
        antinodes++;
    }
    std::println();
    */

    for (const auto & [ x, y ] : overlaps) {
        std::print("({}, {}), ", x, y);
    }
    std::println();

    visualize(overlaps);

    std::println("{} unique antinodes", overlaps.size());
}
