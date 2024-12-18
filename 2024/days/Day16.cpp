#include "Day16.h"

#include "../lib.h"
#include <tuple>
#include <unordered_set>
#include <utility>

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
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

using point = std::pair<int, int>;

using pos = std::tuple<int, int, int>;


point face_to_delta(int f) {
    if (f == 0)
        return std::make_pair(0, -1);
    else if (f == 1)
        return std::make_pair(1, 0);
    else if (f == 2)
        return std::make_pair(0, 1);
    else if (f == 3)
        return std::make_pair(-1, 0);
    std::abort();
}

int rotate_cw(int f) {
    if (f == 3)
        return 0;
    return f + 1;
}

int rotate_ccw(int f) {
    if (f == 0)
        return 3;
    return f - 1;
}

int backtrack(const std::vector<std::vector<char>> &maze, std::unordered_set<pos, tuple_hash> &vis, const point &goal, pos p, int score) {
    const auto px = std::get<0>(p);
    const auto py = std::get<1>(p);

    if (vis.contains(p))
        return INT32_MAX;

    vis.emplace(p);

    if (px == goal.first && py == goal.second) {
        std::println("Found goal with score {}", score);
        return score;
    }

    const auto facing = std::get<2>(p);

    // std::println("Backtracking: px = {}, py = {}, facing = {}", px, py, facing);

    const auto delta = face_to_delta(facing);
    int minScore = INT32_MAX;

    const auto next = std::make_pair(px + delta.first, py + delta.second);

    if (!(next.first < 0 || next.first >= maze[0].size() || next.second < 0 || next.second >= maze.size())) {
        const auto moveT = std::make_tuple(next.first, next.second, facing);

        // try move
        const auto nextV = maze[next.second][next.first];
        if (nextV == '.') {
            const auto tryBack = backtrack(maze, vis, goal, moveT, score + 1);
            if (tryBack < minScore) {
                minScore = tryBack;
            }
        }
    }


    const auto cwT = std::make_tuple(px, py, rotate_cw(facing));
    // try rotate
    const auto try_cw = backtrack(maze, vis, goal, cwT, score + 1000);
    if (try_cw < minScore) {
        minScore = try_cw;
    }


    const auto ccwT = std::make_tuple(px, py, rotate_ccw(facing));
    const auto try_ccw = backtrack(maze, vis, goal, ccwT, score + 1000);
    if (try_ccw < minScore) {
        minScore = try_ccw;
    }

    vis.erase(p);
    return minScore;
}


using path = std::vector<pos>;
struct way_state {
    path way;
    int score;
};

struct CompareWayState {
    bool operator()(const std::pair<int, way_state> &a, const std::pair<int, way_state> &b) const {
        return a.first > b.first;
    }
};

std::vector<path> find_a_path_through_this_wide_world(const std::vector<std::vector<char>> &maze, const pos &start, const point &goal) {
    // track the different ways to get to a pos (lowest)
    std::unordered_map<pos, std::vector<way_state>, tuple_hash> best_scores{};
    std::priority_queue<std::pair<int, way_state>,
                        std::vector<std::pair<int, way_state>>,
                        CompareWayState>
        pq{};

    const auto init_path = std::vector<pos>{start};
    const way_state init_state = {
        .way = init_path,
        .score = 0,
    };
    pq.push({0, init_state});
    best_scores[start] = std::vector<way_state>{init_state};

    int best_score_to_goal = INT32_MAX;
    std::vector<path> best_paths_to_goal;

    while (!pq.empty()) {
        const auto[score, curr_way] = pq.top();
        pq.pop();
        const auto curr = curr_way.way.back();

        if (score > best_score_to_goal)
            break;

        const auto[x, y, f] = curr;

        // did we reach the end? if yes, we found the lowest score by construction
        if (x == goal.first && y == goal.second) {
            if (score < best_score_to_goal) {
                best_score_to_goal = score;
                best_paths_to_goal.clear();
            }
            best_paths_to_goal.push_back(curr_way.way);
            continue;
        }

        const auto delta = face_to_delta(f);
        const auto next_x = x + delta.first;
        const auto next_y = y + delta.second;
        const auto nv = maze[next_y][next_x];

        std::vector<std::pair<pos, int>> candidates{{std::make_tuple(x, y, rotate_cw(f)), score + 1000}, {std::make_tuple(x, y, rotate_ccw(f)), score + 1000}};

        if (nv == '.') {
            candidates.emplace_back(std::make_tuple(next_x, next_y, f), score + 1);
        }

        for (const auto & [ candidate, new_score ] : candidates) {
            auto next_path = curr_way.way;
            next_path.emplace_back(candidate);

            // if we have not evaluated the next move yet or have better score
            if (best_scores[candidate].empty() || new_score <= best_scores[candidate][0].score) {
                if (!best_scores[candidate].empty() && new_score < best_scores[candidate][0].score) {
                    best_scores[candidate].clear();
                }
                best_scores[candidate].push_back({next_path, new_score});
                pq.push({new_score, way_state{next_path, new_score}});
            }
        }
    }

    return best_paths_to_goal;
}


void Day16::run() {
    constexpr auto file_path = "./inputs/day16.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<char>> maze{};
    maze.emplace_back(std::vector<char>{});
    char ch;
    auto idx = 0;

    point p;
    point en;

    while (infile >> std::noskipws >> ch) {

        if (ch == 'S') {
            p = std::make_pair(maze[idx].size(), idx);
            ch = '.';
        } else if (ch == 'E') {
            en = std::make_pair(maze[idx].size(), idx);
            ch = '.';
        }
        if (ch == '\n') {
            idx++;
            maze.emplace_back(std::vector<char>{});
            continue;
        }

        maze[idx].emplace_back(ch);
    }

    //n = 0, e = 1, s = 2, w = 3
    int facing = 1;

    std::println("Maze of size {}x{}; Start at {},{} ; Goal at {},{}", maze[0].size(), maze.size(), p.first, p.second, en.first, en.second);

    // std::unordered_set<pos, tuple_hash> vis{};
    // const auto back = backtrack(maze, vis, en, std::make_tuple(p.first, p.second, facing), 0);

    // std::println("Backtracked to min score of {}", back);
    const auto lowest_scores = find_a_path_through_this_wide_world(maze, std::make_tuple(p.first, p.second, facing), en);
    std::println("Found lowest scores: {}", lowest_scores.size());

    std::unordered_set<point, uwu_hash> best{};
    for (const auto &sc : lowest_scores) {

        for (const auto &pt : sc) {
            best.emplace(std::get<0>(pt), std::get<1>(pt));
        }
    }

    std::println("points on best paths: {}", best.size());
}
