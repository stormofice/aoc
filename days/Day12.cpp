#include "Day12.h"

#include "../lib.h"
#include <unordered_set>

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};
struct uwu_hash2 {
    std::size_t operator()(const std::pair<char, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};


using point = std::pair<int, int>;
// base char and unique id
using region = std::pair<char, int>;
using tregions = std::unordered_map<region, std::unordered_set<point, uwu_hash>, uwu_hash2>;

bool has_region(const tregions &trs, int y, int x) {
    for (const auto &item : trs) {
        if (item.second.contains(std::make_pair(y, x))) {
            return true;
        }
    }
    return false;
}

void determine_region(const std::vector<std::vector<char>> &map, tregions &regions, int sy, int sx) {
    std::vector<point> worklist{};
    worklist.emplace_back(sy, sx);

    const auto height = map[sy].size();
    const auto width = map[0].size();
    char region_base = map[sy][sx];

    if (has_region(regions, sy, sx))
        return;

    int nidx = regions.size() + 1;
    const auto rit = regions.emplace(std::make_pair(region_base, nidx), std::unordered_set<point, uwu_hash>{});
    auto &ps = rit.first->second;

    while (!worklist.empty()) {
        const auto[y, x] = worklist.back();
        worklist.pop_back();

        if (map[y][x] != region_base) {
            continue;
        }

        if (has_region(regions, y, x))
            continue;

        ps.emplace(y, x);

        if (y > 0) {
            worklist.emplace_back(y - 1, x);
        }
        if (x > 0) {
            worklist.emplace_back(y, x - 1);
        }
        if (y < height - 1) {
            worklist.emplace_back(y + 1, x);
        }
        if (x < width - 1) {
            worklist.emplace_back(y, x + 1);
        }
    }
}

void determine_regions(const std::vector<std::vector<char>> &map, tregions &regions) {
    for (auto y = 0; y < map.size(); y++) {
        for (auto x = 0; x < map[y].size(); x++) {
            determine_region(map, regions, y, x);
        }
    }
}

long side_count(const std::unordered_set<point, uwu_hash> &points) {
    long sides = 0;

    std::unordered_map<int, std::vector<int>> ygrouped;

    for (const auto & [ y, x ] : points) {
        ygrouped[y].push_back(x);
    }

    for (auto & [ y, xCoords ] : ygrouped) {
        std::sort(xCoords.begin(), xCoords.end());
    }

    for (const auto & [ y, xCoords ] : ygrouped) {

        for (auto xidx = 0; xidx < xCoords.size(); xidx++) {
            const auto xv = xCoords[xidx];

            if (points.contains(std::make_pair(y - 1, xv)))
                continue;

            int rl = 1;
            for (int k = xidx + 1; k < xCoords.size(); k++) {
                const auto iv = xCoords[k];
                if (xv + rl != iv || points.contains(std::make_pair(y - 1, iv)))
                    break;
                rl++;
            }

            xidx += (rl - 1);

            std::println("[{}] found run length of {} starting at {}", y, rl, xv);
            sides++;
        }
    }

    std::println("y down sc: {}", sides);

    for (const auto & [ y, xCoords ] : ygrouped) {

        for (auto xidx = 0; xidx < xCoords.size(); xidx++) {
            const auto xv = xCoords[xidx];

            if (points.contains(std::make_pair(y + 1, xv)))
                continue;

            int rl = 1;
            for (int k = xidx + 1; k < xCoords.size(); k++) {
                const auto iv = xCoords[k];
                if (xv + rl != iv || points.contains(std::make_pair(y + 1, iv)))
                    break;
                rl++;
            }

            xidx += (rl - 1);

            // std::println("[{}] found run length of {} starting at {}", y, rl, xv);
            sides++;
        }
    }

    std::println("y up sc: {}", sides);


    std::unordered_map<int, std::vector<int>> xgrouped;

    for (const auto & [ y, x ] : points) {
        xgrouped[x].push_back(y);
    }

    for (auto & [ y, yCoords ] : xgrouped) {
        std::sort(yCoords.begin(), yCoords.end());
    }

    for (const auto & [ x, yCoords ] : xgrouped) {

        for (auto yidx = 0; yidx < yCoords.size(); yidx++) {
            const auto yv = yCoords[yidx];

            if (points.contains(std::make_pair(yv, x - 1)))
                continue;

            int rl = 1;
            for (int k = yidx + 1; k < yCoords.size(); k++) {
                const auto iv = yCoords[k];
                if (yv + rl != iv || points.contains(std::make_pair(iv, x - 1)))
                    break;
                rl++;
            }

            yidx += (rl - 1);

            // std::println("[{}] found run length of {} starting at {}", x, rl, yv);
            sides++;
        }
    }

    std::println("x left sc: {}", sides);

    for (const auto &mm : xgrouped[4]) {
        std::print("{}", mm);
    }
    std::println("{} ", xgrouped[4].size());
    std::println();

    for (const auto & [ x, yCoords ] : xgrouped) {

        std::println("@ x = {}", x);

        for (auto yidx = 0; yidx < yCoords.size(); yidx++) {
            const auto yv = yCoords[yidx];

            std::println("@ y = {}", yv);

            if (points.contains(std::make_pair(yv, x + 1)))
                continue;

            int rl = 1;
            for (int k = yidx + 1; k < yCoords.size(); k++) {
                const auto iv = yCoords[k];
                if (yv + rl != iv || points.contains(std::make_pair(iv, x + 1)))
                    break;
                rl++;
            }

            yidx += (rl - 1);

            std::println("[{}] found run length of {} starting at {}", x, rl, yv);
            sides++;
        }
    }

    std::println("x right sc: {}", sides);


    return sides;
}

long checksum(tregions &regions) {
    long chk = 0;
    for (const auto &reg : regions) {
        auto &points = reg.second;
        long area = reg.second.size();

        long perimeter = 0;
        for (const auto & [ y, x ] : points) {
            if (!points.contains(std::make_pair(y + 1, x)))
                perimeter++;
            if (!points.contains(std::make_pair(y - 1, x)))
                perimeter++;
            if (!points.contains(std::make_pair(y, x + 1)))
                perimeter++;
            if (!points.contains(std::make_pair(y, x - 1)))
                perimeter++;
        }

        // std::println("[{}] Area: {}, Perimeter: {} --> {}", reg.first.first, area, perimeter, area * perimeter);
        chk += (area * perimeter);
    }

    return chk;
}

long pt2checksum(tregions &regions) {
    long chk = 0;
    for (const auto &reg : regions) {
        auto &points = reg.second;
        long area = reg.second.size();

        const auto sc = side_count(points);
        std::println("[{}] Area: {}, SC: {} --> {}", reg.first.first, area, sc, area * sc);
        chk += (area * sc);
    }

    return chk;
}

void Day12::run() {
    constexpr auto file_path = "./inputs/day12.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<char>> map{};
    map.emplace_back(std::vector<char>{});

    // std::unordered_map<point, region, uwu_hash> regions{};
    tregions regions{};

    char ch;
    auto idx = 0;
    while (infile >> std::noskipws >> ch) {
        if (ch == '\n') {
            map.emplace_back(std::vector<char>{});
            idx++;
            continue;
        }
        map[idx].emplace_back(ch);
    }

    for (const auto &item : map) {
        for (const auto &item2 : item) {
            std::print("{}", item2);
        }
        std::println();
    }

    determine_regions(map, regions);

    std::println("Found {} unique regions", regions.size());
    std::println("Checksum: {}", checksum(regions));
    std::println("Checksum2: {}", pt2checksum(regions));
}
