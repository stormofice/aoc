#include "Day18.h"

#include "../lib.h"

#include <cstring>

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};

constexpr auto width = 71;
constexpr auto height = 71;

void print_grid(std::vector<std::vector<char>> &grid, const int width, const int height) {
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            std::print("{}", grid[y][x]);
        }
        std::println();
    }
}

int backtrack(const char grid[][width], std::unordered_set<std::pair<int, int>, uwu_hash> &vis, int x, int y, int score) {
    if (vis.contains(std::make_pair(x, y)))
        return -1;
    if (x < 0 || y < 0 || x >= width || y >= height)
        return -1;
    if (grid[y][x] == '#')
        return -1;
    if (x == width - 1 && y == height - 1)
        return score;


    vis.emplace(x, y);


    std::vector<std::pair<int, int>> candidates{};
    candidates.emplace_back(1, 0);
    candidates.emplace_back(-1, 0);
    candidates.emplace_back(0, 1);
    candidates.emplace_back(0, -1);

    int lowest = -1;
    for (const auto & [ dx, dy ] : candidates) {

        const auto bt = backtrack(grid, vis, x + dx, y + dy, score + 1);
        if (bt != -1 && (lowest == -1 || bt < lowest)) {
            lowest = bt;
        }
    }

    vis.erase(std::make_pair(x, y));


    return lowest;
}

int cringe_queue(std::vector<std::vector<char>> &grid) {
    std::unordered_map<std::pair<int, int>, int, uwu_hash> bests{};
    std::queue<std::tuple<int, int, int>> q{};

    q.emplace(0, 0, 0);

    while (q.size() > 0) {
        const auto[x, y, score] = q.front();
        q.pop();

        // std::println("trying {} {}", x, y);

        if (x == width - 1 && y == height - 1)
            return score;

        std::vector<std::pair<int, int>> candidates{};
        candidates.emplace_back(1, 0);
        candidates.emplace_back(-1, 0);
        candidates.emplace_back(0, 1);
        candidates.emplace_back(0, -1);


        for (const auto & [ dx, dy ] : candidates) {
            const auto nx = x + dx;
            const auto ny = y + dy;

            if (ny >= 0 && nx >= 0 && ny < height && nx < width && grid[ny][nx] != '#') {
                const auto new_score = score + 1;
                const auto next_pos = std::make_pair(nx, ny);

                if (!bests.contains(next_pos) || new_score < bests[next_pos]) {
                    bests[next_pos] = new_score;
                    q.emplace(nx, ny, new_score);
                }
            }
        }
    }
    return bests[std::make_pair(width - 1, height - 1)];
}

void Day18::run() {
    constexpr auto file_path = "./inputs/day18.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<char>> grid{};
    for (auto y = 0; y < height; y++) {
        grid.emplace_back(std::vector<char>{});
        for (auto x = 0; x < width; x++)
            grid[y].emplace_back('.');
    }

    auto idx = 0;

    int digit;
    int x = -1, y = -1;
    char ch;

    while (infile >> digit) {
        if (idx % 2 == 0) {
            x = digit;
            infile >> ch;
        } else {
            y = digit;
            grid[y][x] = '#';
        }
        idx++;

        /*
        if ((idx) >= (1024 * 2)) {
            std::println("ending at {} {}", x, y);
            break;
        }*/
        if ((idx) >= (1024 * 2)) {
            const auto cq = cringe_queue(grid);
            if (cq == 0) {
                std::println("unreachable after {}, {} {}", idx, x, y);
                break;
            }
        }
    }

    print_grid(grid, width, height);

    // auto vis = std::unordered_set<std::pair<int, int>, uwu_hash>{};
    // const auto pl = backtrack(grid, vis, 0, 0, 0);
    // std::println("backtrack: {}", pl);
    const auto cq = cringe_queue(grid);
    std::println("cq: {}", cq);
}
