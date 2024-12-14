#include "Day10.h"

#include "../lib.h"
#include <utility>

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};

using map = std::vector<std::vector<char>>;

std::pair<int, int> no_trails(const map &map, int sy, int sx) {

    using attempt = std::pair<char, std::pair<int, int>>;
    std::vector<attempt> attempts{};

    attempts.emplace_back('0', std::make_pair(sy, sx));

    std::unordered_set<std::pair<int, int>, uwu_hash> reached_trailheads{};

    int no = 0;

    while (!attempts.empty()) {
        // Grattelsprache
        const attempt att = attempts.back();
        attempts.pop_back();

        const auto y = att.second.first;
        const auto x = att.second.second;

        // std::println("[{},{}], requiring {}, looking at {},{} --'ch@{}", sy, sx, att.first, y, x, map[y][x]);

        if (att.first != map[y][x]) {
            continue;
        }

        if (att.first == '9') {
            no++;
            reached_trailheads.emplace(y, x);
            continue;
        }

        char nh = att.first + 1;
        if (y > 0) {
            attempts.emplace_back(nh, std::make_pair(y - 1, x));
        }
        if (y < (map.size() - 1)) {
            attempts.emplace_back(nh, std::make_pair(y + 1, x));
        }
        if (x > 0) {
            attempts.emplace_back(nh, std::make_pair(y, x - 1));
        }
        if (x < (map[0].size() - 1)) {
            attempts.emplace_back(nh, std::make_pair(y, x + 1));
        }
    }

    return std::make_pair(reached_trailheads.size(), no);
}

std::pair<int, int> find_all_trails(const map &map) {
    int tc = 0;
    int dc = 0;
    for (auto y = 0; y < map.size(); y++) {
        for (auto x = 0; x < map[0].size(); x++) {
            if (map[y][x] == '0') {
                // std::println("searching for trails at {} {}", y, x);
                const auto tt = no_trails(map, y, x);
                tc += tt.first;
                dc += tt.second;
            }
        }
    }

    return std::make_pair(tc, dc);
}

void Day10::run() {
    constexpr auto file_path = "./inputs/day10.input";
    std::ifstream infile(file_path);

    map map{};

    int x = 0, y = 0;
    char ch;
    map.emplace_back(std::vector<char>{});
    while (infile >> std::noskipws >> ch) {
        if (ch == '\n') {
            y++;
            map.emplace_back(std::vector<char>{});
            continue;
        }

        map[y].emplace_back(ch);
    }

    for (const auto &o : map) {
        for (const auto &item : o) {
            std::print("{}", item);
        }
        std::println();
    }

    const auto r = find_all_trails(map);
    std::println("Trail score of {}, Number of distinct trails  {}", r.first, r.second);
}
