#include "Day15.h"

#include "../lib.h"
#include <vector>

using row = std::vector<char>;
using map = std::vector<row>;
using point = std::pair<int, int>;
using delta = point;

struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};


void print_map(const map &map) {
    for (auto y = 0; y < map.size(); y++) {
        for (auto x = 0; x < map[y].size(); x++) {
            std::print("{}", map[y][x]);
        }
        std::println();
    }
}

void print_moves(const std::vector<char> &moves) {
    for (const auto &item : moves) {
        std::print("{}", item);
    }
    std::println();
}

point move_robot(map &map, const point &robot, const delta &move) {
    int ny = robot.second + move.second;
    int nx = robot.first + move.first;


    char n_ch = map[ny][nx];

    // std::println("MC: {} ({}, {})", n_ch, nx, ny);

    // easy case
    if (n_ch == '#') {
        return robot;
    }
    if (n_ch == '.') {
        map[robot.second][robot.first] = n_ch;
        map[ny][nx] = '@';
        return std::make_pair(nx, ny);
    }
    if (n_ch != 'O') {
        std::abort();
    }

    int direction = 0;
    if (move.first > 0 || move.second > 0) {
        direction = 1;
    } else {
        direction = -1;
    }
    bool move_x = move.first != 0;

    // scan in push direction
    auto start_idx = (move_x ? nx : ny);

    for (auto idx = start_idx; direction > 0 ? (idx < (move_x ? map[robot.second].size() : map.size())) : idx > 0; idx += direction) {
        const auto look = map[move_x ? ny : idx][move_x ? idx : nx];

        if (look == '.') {
            // move all boxes
            map[move_x ? ny : idx][move_x ? idx : nx] = 'O';
            map[move_x ? ny : start_idx][move_x ? start_idx : nx] = '@';
            map[robot.second][robot.first] = '.';
            return std::make_pair(nx, ny);
        }

        // couldn't find spot to push too
        if (look == '#') {
            return robot;
        }
    }

    // huh, weird case, but happens quite often
    // should? occur when trying to move oob
    return robot;
}


// completely unnecessary, could also be done in the move fct; left in to honor the approach
bool check_wide_v_move(const map &map, int bx, int by, int direction) {
    const auto og_ch = map[by][bx];

    if (og_ch == '.') {
        return true;
    } else if (og_ch == '#') {
        return false;
    }

    int lbx, rbx;
    if (og_ch == '[') {
        lbx = bx;
        rbx = lbx + 1;
    } else {
        rbx = bx;
        lbx = bx - 1;
    }

    auto left = check_wide_v_move(map, lbx, by + direction, direction);
    auto right = check_wide_v_move(map, rbx, by + direction, direction);

    return left && right;
}

void collect_boxes(const map &prev, int bx, int by, int direction, std::unordered_set<point, uwu_hash> &affected_boxes) {
    const auto ch = prev[by][bx];

    if (ch == '.' || ch == '#')
        return;

    if (ch == '[') {
        affected_boxes.emplace(bx, by);
        collect_boxes(prev, bx, by + direction, direction, affected_boxes);
        collect_boxes(prev, bx + 1, by + direction, direction, affected_boxes);

    } else {
        affected_boxes.emplace(bx - 1, by);
        collect_boxes(prev, bx - 1, by + direction, direction, affected_boxes);
        collect_boxes(prev, bx, by + direction, direction, affected_boxes);
    }
}

bool wide_v_move(const map &prev, map &map, int bx, int by, int direction) {
    std::unordered_set<point, uwu_hash> affected_boxes{};
    collect_boxes(prev, bx, by, direction, affected_boxes);

    // std::println("{} affected", affected_boxes.size());

    auto unique = std::vector<point>(affected_boxes.begin(), affected_boxes.end());
    std::sort(unique.begin(), unique.end(), [&](point &f, point &s) {
        return direction < 0 ? f.second < s.second : f.second > s.second;
    });

    for (const auto &box : unique) {
        // std::println("Moving box ({},{})", box.first, box.second);
        map[box.second + direction][box.first] = '[';
        map[box.second + direction][box.first + 1] = ']';
        map[box.second][box.first] = '.';
        map[box.second][box.first + 1] = '.';
    }

    return true;
}

/*
bool wide_v_move(const map &prev, map &map, int bx, int by, int direction, char you) {
    const auto og_ch = prev[by][bx];

    std::println("mv: og {} bx {} by {}", og_ch, bx, by);
    if (og_ch == '.') {
        map[by][bx] = you;
        return true;
    } else if (og_ch == '#') {
        // this cannot occur, as we check before
        return false;
        std::println("invariant violated?");
        std::abort();
    }


    bool succ = false;
    if (og_ch == '[') {
        const auto lm = wide_v_move(prev, map, bx, by + direction, direction, '[');
        const auto rm = wide_v_move(prev, map, bx + 1, by + direction, direction, ']');
        succ = lm && rm;
        if (succ) {
            map[by][bx] = '.';
            map[by][bx + 1] = '.';
        }
    } else if (og_ch == ']') {
        const auto lm = wide_v_move(prev, map, bx - 1, by + direction, direction, '[');
        const auto rm = wide_v_move(prev, map, bx, by + direction, direction, ']');
        succ = lm && rm;
        if (succ) {
            map[by][bx - 1] = '.';
            map[by][bx] = '.';
        }
    }

    return succ;
}
*/

point move_robot_in_a_wide_world(map &map, const point &robot, const delta &move) {
    int ny = robot.second + move.second;
    int nx = robot.first + move.first;
    char n_ch = map[ny][nx];

    // std::println("{} {} {}", n_ch, nx, ny);
    // easy case
    if (n_ch == '#') {
        return robot;
    }
    if (n_ch == '.') {
        map[robot.second][robot.first] = n_ch;
        map[ny][nx] = '@';
        return std::make_pair(nx, ny);
    }
    if (n_ch != '[' && n_ch != ']') {
        std::println("bye?");
        std::abort();
    }

    int direction = 0;
    if (move.first > 0 || move.second > 0) {
        direction = 1;
    } else {
        direction = -1;
    }
    bool move_x = move.first != 0;

    // easy case
    if (move_x) {
        for (auto idx = nx; direction > 0 ? idx < map[robot.second].size() : idx > 0; idx += direction) {
            const auto look = map[robot.second][idx];

            // couldn't find spot to push too
            if (look == '#') {
                return robot;
            }

            if (look == '.') {
                map[robot.second][robot.first] = '.';
                bool flip = false;
                for (auto bi = nx + direction; direction > 0 ? bi <= idx : bi >= idx; bi += direction) {
                    if (!flip) {
                        map[robot.second][bi] = direction < 0 ? ']' : '[';
                    } else {
                        map[robot.second][bi] = direction < 0 ? '[' : ']';
                    }
                    flip = !flip;
                }
                map[ny][nx] = '@';
                return std::make_pair(nx, ny);
            }
        }
    } else {
        // span up a tree and check if all boxes can be moved

        const auto prev = map;

        const auto lm = n_ch == '[' ? check_wide_v_move(map, nx, ny, direction) : check_wide_v_move(map, nx - 1, ny, direction);
        const auto rm = n_ch == ']' ? check_wide_v_move(map, nx, ny, direction) : check_wide_v_move(map, nx + 1, ny, direction);
        // std::println("left move: {}, right move: {}", lm, rm);
        if (lm && rm) {
            // std::println("moving possible, locking in..");
            wide_v_move(prev, map, n_ch == '[' ? nx : nx - 1, ny, direction);

            map[robot.second][robot.first] = '.';
            map[ny][nx] = '@';

            return std::make_pair(nx, ny);
        }
    }


    // huh, weird case, but happens quite often
    // should? occur when trying to move oob
    return robot;
}

map scale_up(const map &in_map) {
    map m{};
    m.emplace_back(row{});
    for (auto y = 0; y < in_map.size(); y++) {
        for (auto x = 0; x < in_map[y].size(); x++) {
            const auto ch = in_map[y][x];
            if (ch == '#') {
                m[y].emplace_back('#');
                m[y].emplace_back('#');
            } else if (ch == '@') {
                m[y].emplace_back('@');
                m[y].emplace_back('.');
            } else if (ch == '.') {
                m[y].emplace_back('.');
                m[y].emplace_back('.');
            } else if (ch == 'O') {
                m[y].emplace_back('[');
                m[y].emplace_back(']');
            }
        }
        m.emplace_back(row{});
    }
    return m;
}

long checksum(const map &map) {
    long sum = 0;
    for (auto y = 0; y < map.size(); y++) {
        for (auto x = 0; x < map[y].size(); x++) {
            const auto ch = map[y][x];
            if (ch == 'O' || ch == '[') {
                sum += (y * 100) + x;
            }
        }
    }
    return sum;
}

delta get_move(char c) {
    switch (c) {
    case 'v':
        return std::make_pair(0, 1);
    case '^':
        return std::make_pair(0, -1);
    case '>':
        return std::make_pair(1, 0);
    case '<':
        return std::make_pair(-1, 0);
    default:
        std::abort();
    }
}

void Day15::run() {
    constexpr auto file_path = "./inputs/day15.input";
    std::ifstream infile(file_path);


    map map{};
    map.emplace_back(row{});

    char ch;
    auto idx = 0;

    point r_pos = std::make_pair(0, 0);
    int rc = 0;

    std::vector<char> moves{};
    bool read_moves = false;

    while (infile >> std::noskipws >> ch) {
        if (ch == '\n') {

            if (rc == 0) {
                read_moves = true;
            }

            if (!read_moves) {
                map.emplace_back(row{});
                idx++;
                rc = 0;
            }
            continue;
        }

        if (read_moves) {
            moves.emplace_back(ch);
            continue;
        }

        if (ch == '@') {
            r_pos.first = map[idx].size();
            r_pos.second = map.size() - 1;
        }

        map[idx].emplace_back(ch);
        rc++;
    }

    // too lazy to fix properly
    if (map[map.size() - 1].size() == 0) {
        map.pop_back();
    }

    std::println("Map is {}x{} tiles large, Robot starts at: {},{}", map[0].size(), map.size(), r_pos.first, r_pos.second);
    print_map(map);
    std::print("Moves (#{}): ", moves.size());
    print_moves(moves);

    // pt2 copies
    auto scaled = scale_up(map);
    auto r_scaled = std::make_pair(r_pos.first * 2, r_pos.second);

    for (const auto &move : moves) {
        std::print("{}", move);
        const auto d = get_move(move);
        r_pos = move_robot(map, r_pos, d);
        // print_map(map);
    }

    std::println();

    const auto chk = checksum(map);
    std::println("Checksum after {} moves: {}", moves.size(), chk);

    std::println("---");
    print_map(scaled);

    std::println("Wide world, robot at {},{}", r_scaled.first, r_scaled.second);

    for (const auto &move : moves) {
        // std::print("{}", move);
        const auto d = get_move(move);
        r_scaled = move_robot_in_a_wide_world(scaled, r_scaled, d);
        // print_map(scaled);
    }
    const auto wchk = checksum(scaled);
    std::println("Checksum after {} moves: {}", moves.size(), wchk);
}
