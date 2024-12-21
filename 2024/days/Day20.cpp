#include "Day20.h"

#include "../lib.h"
#include <unordered_map>

using point = std::pair<int, int>;
struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};

std::vector<point> dij(const std::vector<std::vector<char>> &map, const point &start, const point &end) {
    using entry = std::tuple<point, std::vector<point>>;
    std::queue<entry> q{};
    q.push({start, {start}});

    std::unordered_set<point, uwu_hash> vis{};

    while (q.size() > 0) {
        const auto[curr, path] = q.front();
        q.pop();

        if (curr.first == end.first && curr.second == end.second) {
            return path;
        }

        if (vis.contains(curr))
            continue;
        vis.emplace(curr);

        const std::vector<point> deltas{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        for (const auto &delta : deltas) {
            const auto np = std::make_pair(curr.first + delta.first, curr.second + delta.second);
            if (map[np.second][np.first] == '#')
                continue;
            auto nw = path;
            nw.emplace_back(np);
            q.emplace(np, nw);
        }
    }

    assert(false);
    return {};
}

std::unordered_map<int, int> find_cheats_on_path(const std::vector<point> &path) {
    std::unordered_map<int, int> cheats{};

    int fi = 0, ei = 0;
    for (const auto & [ bx, by ] : path) {
        for (const auto & [ ex, ey ] : path) {
            if ((bx == ex && std::abs(by - ey) == 2) || (by == ey && std::abs(bx - ex) == 2)) {
                int cuts = ei - fi - 2;
                std::println("jo kollege");
                if (cuts > 0) {
                    if (cheats.contains(cuts))
                        cheats[cuts]++;
                    else
                        cheats.emplace(cuts, 1);
                }
            }
            ei++;
        }
        ei = 0;
        fi++;
    }

    return cheats;
}

std::unordered_map<int, int> find_unhinged_cheats_on_path(const std::vector<point> &path) {
    std::unordered_map<int, int> cheats{};

    int fi = 0, ei = 0;
    for (const auto & [ bx, by ] : path) {
        for (const auto & [ ex, ey ] : path) {
            const auto delta = std::abs(by - ey) + std::abs(bx - ex);
            if (delta <= 20) {
                int cuts = ei - fi - delta;
                if (cuts > 0) {
                    if (cheats.contains(cuts))
                        cheats[cuts]++;
                    else
                        cheats.emplace(cuts, 1);
                }
            }
            ei++;
        }
        ei = 0;
        fi++;
    }

    return cheats;
}

void Day20::run() {
    constexpr auto file_path = "./inputs/day20.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<char>> map{};
    map.emplace_back(std::vector<char>{});
    char ch;
    auto idx = 0;

    point start;
    point end;

    while (infile >> std::noskipws >> ch) {
        if (ch == 'S') {
            start = std::make_pair(map[idx].size(), idx);
            ch = '.';
        } else if (ch == 'E') {
            end = std::make_pair(map[idx].size(), idx);
            ch = '.';
        }
        if (ch == '\n') {
            idx++;
            map.emplace_back(std::vector<char>{});
            continue;
        }
        map[idx].emplace_back(ch);
    }

    std::println("Map: {}x{}; Start {},{}; Finish: {},{}", map[0].size(), map.size(), start.first, start.second, end.first, end.second);

    const auto npath = dij(map, start, end);
    std::println("Normal path length: {}", npath.size());

    const auto cheats = find_cheats_on_path(npath);
    for (const auto & [ k, v ] : cheats) {
        std::println("{} cheats of length {}", v, k);
    }

    const auto pt1_cheats = std::accumulate(cheats.begin(), cheats.end(), 0, [](int acc, std::pair<int, int> kv) {
        if (kv.first >= 100)
            return acc + kv.second;
        return acc;
    });
    std::println("{} cheats would save at least 100 picoseconds", pt1_cheats);

    const auto unhinged_cheats = find_unhinged_cheats_on_path(npath);
    for (const auto & [ k, v ] : unhinged_cheats) {
        std::println("{} unhinged cheats of length {}", v, k);
    }

    const auto pt2_cheats = std::accumulate(unhinged_cheats.begin(), unhinged_cheats.end(), 0, [](int acc, std::pair<int, int> kv) {
        if (kv.first >= 100)
            return acc + kv.second;
        return acc;
    });
    std::println("{} unhinged cheats would save at least 100 picoseconds", pt2_cheats);
}
