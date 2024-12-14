#include "Day14.h"

#include "../lib.h"
#include <string>
#include <unordered_map>

using point = std::pair<int, int>;
using vec = point;
struct Robot {
    point pos;
    vec move;
};

// languages without proper mod xdd
inline int mod(int a, int b) {
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}


void simulate_step(std::vector<Robot> &robots, int w, int h) {
    for (auto &r : robots) {
        r.pos.first = mod((r.pos.first + r.move.first), w);
        r.pos.second = mod((r.pos.second + r.move.second), h);
    }
}

std::string get_robot_map(const std::vector<Robot> &robots, int w, int h) {
    std::string res = "";

    for (auto y = 0; y < h; y++) {
        for (auto x = 0; x < w; x++) {
            const auto rc = std::count_if(robots.begin(), robots.end(), [y, x](const Robot &r) {
                return y == r.pos.second && x == r.pos.first;
            });
            if (rc == 0)
                res += '.';
            else
                res += std::to_string(rc);
        }
        res += '\n';
    }

    return res;
}

long checksum(const std::vector<Robot> &robots, int w, int h) {
    int wsplit = w / 2;
    int hsplit = h / 2;

    std::println("wsplit: {}, hsplit: {}", wsplit, hsplit);

    int q0 = 0,
        q1 = 0, q2 = 0, q3 = 0;

    for (const auto &r : robots) {

        if (r.pos.first < wsplit) {
            if (r.pos.second < hsplit) {
                q0++;
            } else if (r.pos.second > hsplit) {
                q1++;
            }
        } else if (r.pos.first > wsplit) {
            if (r.pos.second < hsplit) {
                q2++;
            } else if (r.pos.second > hsplit) {
                q3++;
            }
        }
    }

    std::println("{},{},{},{}", q0, q1, q2, q3);

    return q0 * q1 * q2 * q3;
}

bool heuristic_hit(const std::vector<Robot> &robots, int w, int h) {
    // heuristic idea: if a certain row or column has significantly more robots than the average, something might be off
    std::unordered_map<int, int> xs{};
    std::unordered_map<int, int> ys{};


    for (const auto &r : robots) {
        if (xs.find(r.pos.first) != xs.end()) {
            xs[r.pos.first]++;
        } else {
            xs[r.pos.first] = 1;
        }

        if (ys.find(r.pos.second) != ys.end()) {
            ys[r.pos.second]++;
        } else {
            ys[r.pos.second] = 1;
        }
    }

    auto xavg = robots.size() / w;
    auto yavg = robots.size() / h;

    constexpr auto interesting = 5;

    for (const auto & [ k, v ] : xs) {
        if (v > (xavg * interesting))
            return true;
    }

    for (const auto & [ k, v ] : ys) {
        if (v > (yavg * interesting))
            return true;
    }

    return false;
}

void Day14::run() {
    constexpr auto file_path = "./inputs/day14.input";
    std::ifstream infile(file_path);

    std::vector<Robot> robots{};

    char ch;
    auto idx = 0;
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        //p=
        iss >> ch >> ch;
        int px, py, vx, vy;
        iss >> px;
        // ,
        iss >> ch;
        iss >> py;
        // v=
        iss >> ch >> ch;
        iss >> vx;
        iss >> ch;
        iss >> vy;
        const auto r = Robot{
            .pos = std::make_pair(px, py),
            .move = std::make_pair(vx, vy),
        };
        robots.emplace_back(r);
    }

    int w = 101, h = 103;
    std::println("{}", get_robot_map(robots, w, h));

    auto rs = robots;

    constexpr auto step_count = 100;
    for (auto si = 0; si < step_count; si++) {
        simulate_step(robots, w, h);
    }
    std::println("{}", get_robot_map(robots, w, h));

    std::println("Checksum after {} steps: {}", step_count, checksum(robots, w, h));

    // search for tree
    constexpr auto search_count = 500000;
    std::ofstream save_file;
    save_file.open("sims.txt", std::ios_base::app);
    for (auto si = 0; si < search_count; si++) {
        simulate_step(rs, w, h);
        if (heuristic_hit(rs, w, h)) {
            save_file << "STEP COUNT: " << std::to_string(si) << std::endl;
            save_file << get_robot_map(rs, w, h);
            save_file << "#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#" << std::endl;
            // std::cin.get();
        }
    }
}
