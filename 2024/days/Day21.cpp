#include "Day21.h"

#include "../lib.h"
#include <unordered_map>

// right, left, up, down
// using presses = std::tuple<int, int, int, int>;
// nuh uh, need sequentiality to prevent going over gap
using presses = std::vector<char>;


// number -> <other number, presses to get to>
// e.g.
// to get from 0 -> 5, you have to the n presses of each


const std::unordered_map<char, std::pair<int, int>> num_coords{
    {'A', {0, 0}},
    {'0', {1, 0}},
    {'1', {2, 1}},
    {'2', {1, 1}},
    {'3', {0, 1}},
    {'4', {2, 2}},
    {'5', {1, 2}},
    {'6', {0, 2}},
    {'7', {2, 3}},
    {'8', {1, 3}},
    {'9', {0, 3}},
};

const std::unordered_map<char, std::pair<int, int>> dir_coords{
    {'>', {0, -1}},
    {'v', {1, -1}},
    {'<', {2, -1}},
    {'^', {1, 0}},
    {'A', {0, 0}},
};

std::unordered_map<char, std::unordered_map<char, presses>> gen_number_map() {
    std::unordered_map<char, std::unordered_map<char, presses>> how{};

    const auto chs = std::views::keys(num_coords);

    for (const auto &ch : chs) {
        std::unordered_map<char, presses> cmap{};

        const auto src = num_coords.at(ch);

        for (const auto &c : chs) {
            presses p{};

            // if they are the same, just stay
            if (c != ch) {

                const auto dest = num_coords.at(c);

                const auto xdiff = src.first - dest.first;
                const auto ydiff = src.second - dest.second;

                // xdiff > 0 -> move right
                // ydiff > 0 -> move down

                if ((ch == 'A' || ch == '0') && (c == '1' || c == '4' || c == '7')) {
                    if (ydiff != 0) {
                        for (auto i = 0; i < std::abs(ydiff); i++) {
                            if (ydiff > 0) {
                                p.emplace_back('v');
                            } else {
                                p.emplace_back('^');
                            }
                        }
                    }

                    if (xdiff != 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            if (xdiff > 0) {
                                p.emplace_back('>');
                            } else {
                                p.emplace_back('<');
                            }
                        }
                    }

                } else {

                    if (xdiff < 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            p.emplace_back('<');
                        }
                    }

                    if (ydiff != 0) {
                        for (auto i = 0; i < std::abs(ydiff); i++) {
                            if (ydiff > 0) {
                                p.emplace_back('v');
                            } else {
                                p.emplace_back('^');
                            }
                        }
                    }

                    if (xdiff > 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            p.emplace_back('>');
                        }
                    }
                }
            }

            // todo
            p.emplace_back('A');
            cmap.emplace(c, p);
        }
        how.emplace(ch, cmap);
    }

    return how;
}


std::unordered_map<char, std::unordered_map<char, presses>> gen_dir_map() {
    std::unordered_map<char, std::unordered_map<char, presses>> how{};

    const auto chs = std::views::keys(dir_coords);

    for (const auto &ch : chs) {
        std::unordered_map<char, presses> cmap{};

        const auto src = dir_coords.at(ch);

        for (const auto &c : chs) {
            presses p{};

            // if they are the same, just stay
            if (c != ch) {

                const auto dest = dir_coords.at(c);

                const auto xdiff = src.first - dest.first;
                const auto ydiff = src.second - dest.second;

                // xdiff > 0 -> move right
                // ydiff > 0 -> move down

                if ((ch == '^' || ch == 'A') && (c == '<')) {
                    if (ydiff != 0) {
                        for (auto i = 0; i < std::abs(ydiff); i++) {
                            if (ydiff > 0) {
                                p.emplace_back('v');
                            } else {
                                p.emplace_back('^');
                            }
                        }
                    }

                    if (xdiff != 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            if (xdiff > 0) {
                                p.emplace_back('>');
                            } else {
                                p.emplace_back('<');
                            }
                        }
                    }
                } else {

                    if (xdiff < 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            p.emplace_back('<');
                        }
                    }

                    if (ydiff != 0) {
                        for (auto i = 0; i < std::abs(ydiff); i++) {
                            if (ydiff > 0) {
                                p.emplace_back('v');
                            } else {
                                p.emplace_back('^');
                            }
                        }
                    }

                    if (xdiff > 0) {
                        for (auto i = 0; i < std::abs(xdiff); i++) {
                            p.emplace_back('>');
                        }
                    }
                }
            }

            // todo
            p.emplace_back('A');
            cmap.emplace(c, p);
        }
        how.emplace(ch, cmap);
    }

    return how;
}

// ========== map gen end

void print_gen_map(const std::unordered_map<char, std::unordered_map<char, presses>> &map) {
    for (const auto & [ ch, chmap ] : map) {
        std::println("map for {}", ch);
        for (const auto & [ to, presses ] : chmap) {
            std::print("{}: ", to);
            for (const auto &p : presses) {
                std::print("{}", p);
            }
            std::println();
        }
    }
}

using ck = std::pair<int, char>;
struct uwu_hash {
    std::size_t operator()(const std::pair<int, char> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};


void generate_sequence(
    // input seq
    const std::vector<char> &target_sequence,
    // gen maps
    const std::unordered_map<char, std::unordered_map<char, presses>> &num_map,
    const std::unordered_map<char, std::unordered_map<char, presses>> &dir_map,
    int depth,
    // current buttons
    std::vector<char> &current_states,
    presses &accumulated_moves) {

    // num level base case
    if (depth == 0) {
        for (size_t i = 0; i < target_sequence.size(); i++) {
            const auto &moves = num_map.at(current_states[0]).at(target_sequence[i]);

            // append all moves required to get to this button
            accumulated_moves.insert(accumulated_moves.end(), moves.begin(), moves.end());
            accumulated_moves.push_back('A');

            // update num state
            current_states[0] = target_sequence[i];
        }
        return;
    }

    // go down to get base sequence (as acc starts empty)
    presses base_sequence;
    generate_sequence(target_sequence, num_map, dir_map, depth - 1, current_states, base_sequence);

    // go through all levels
    presses control_sequence;
    for (const auto &move : base_sequence) {
        const auto &moves = dir_map.at(current_states[depth]).at(move);
        control_sequence.insert(control_sequence.end(), moves.begin(), moves.end());
        control_sequence.push_back('A');
        current_states[depth] = move;
    }

    // update with new moves
    accumulated_moves = control_sequence;
}

std::unordered_map<ck, unsigned long long, uwu_hash> cache{};


unsigned long long memmem(const std::unordered_map<char, std::unordered_map<char, presses>> &num_map,
                          const std::unordered_map<char, std::unordered_map<char, presses>> &dir_map,
                          char curr,
                          int depth) {

    const auto cck = std::make_pair(depth, curr);
    if (cache.contains(cck))
        return cache[cck];

    if (depth == 0) {
        return num_map.at(curr).size();
    }

    char s = 'A';
    auto acc = 0ULL;

    if (depth > 0) {
        const auto moves = dir_map.at(s).at(curr);
        for (const auto &m : moves) {
            acc += memmem(num_map, dir_map, m, depth - 1);
            s = m;
        }
    }

    cache.emplace(cck, acc);

    return acc;
}

struct mem_hash {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = hash_vector(p.second);
        return h1 ^ (h2 << 1); // Combine hashes
    }

  private:
    size_t hash_vector(const std::vector<char> &vec) const {
        size_t hash = vec.size();
        for (const char &i : vec) {
            // Combine each element's hash using a prime number
            hash ^= std::hash<char>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

std::unordered_map<std::pair<int, std::vector<char>>, unsigned long long, mem_hash> mem{};
unsigned long long fp(
    const std::unordered_map<char, std::unordered_map<char, presses>> &num_map, std::vector<char> sequence, int depth) {
    if (depth == 0) {
        return sequence.size();
    }

    if (mem.count({depth, sequence})) {
        return mem.at({depth, sequence});
    }

    char drag{'A'};
    unsigned long long pathLength = 0;
    for (char c : sequence) {
        pathLength += fp(num_map, num_map.at(drag).at(c), depth - 1);
        drag = c;
    }

    mem.insert({{depth, sequence}, pathLength});
    return pathLength;
}

unsigned long long perform_single_run(
    const std::unordered_map<char, std::unordered_map<char, presses>> &num_map,
    const std::unordered_map<char, std::unordered_map<char, presses>> &dir_map,
    const std::vector<char> &input,
    int sc) {

    return fp(num_map, input, sc);
}


void Day21::run() {
    const auto nmap = gen_number_map();
    print_gen_map(nmap);
    const auto dmap = gen_dir_map();
    print_gen_map(dmap);

    std::println(".");
    std::unordered_map<char, std::unordered_map<char, presses>> comb;
    for (auto & [ k, v ] : nmap) {
        if (!comb.contains(k))
            comb.emplace(k, std::unordered_map<char, presses>{});

        auto &im = comb[k];
        for (auto & [ ik, iv ] : v) {
            if (!im.contains(ik)) {
                im.emplace(ik, std::vector<char>{});
            }
            for (auto cv : iv)
                im[ik].emplace_back(cv);
        }
    }
    for (auto & [ k, v ] : dmap) {
        if (!comb.contains(k))
            comb.emplace(k, std::unordered_map<char, presses>{});

        auto &im = comb[k];
        for (auto & [ ik, iv ] : v) {
            if (!im.contains(ik)) {
                im.emplace(ik, std::vector<char>{});
            }
            for (auto cv : iv)
                im[ik].emplace_back(cv);
        }
    }
    comb['A']['A'] = {'A'};
    print_gen_map(comb);


    /*
    std::vector<std::vector<char>> inputs{{'0', '2', '9', 'A'},
                                          {'9', '8', '0', 'A'},
                                          {'1', '7', '9', 'A'},
                                          {'4', '5', '6', 'A'},
                                          {'3', '7', '9', 'A'}};
    std::vector<unsigned long long> numerics{29, 980, 179, 456, 378};
    */
    std::vector<std::vector<char>> inputs{{'3', '1', '9', 'A'},
                                          {'0', '8', '5', 'A'},
                                          {'1', '4', '3', 'A'},
                                          {'2', '8', '6', 'A'},
                                          {'7', '8', '9', 'A'}};
    std::vector<unsigned long long> numerics{319, 85, 143, 286, 789};

    constexpr auto sc = 26;

    auto complexity = 0ULL;
    auto idx = 0;

    for (const auto &input : inputs) {
        const auto l = perform_single_run(comb, dmap, input, sc);
        std::println("complexity += ( {} * {} )", l, numerics[idx]);
        complexity += (l * numerics[idx]);
        idx++;
    }

    std::println("final complexity: {}", complexity);
}
