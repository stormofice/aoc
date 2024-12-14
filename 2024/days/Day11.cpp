#include "Day11.h"

#include "../lib.h"
struct uwu_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};
using stone = long;

std::vector<long> get_digits_old(long no) {
    std::vector<long> digits{};

    if (no == 0) {
        return std::vector<long>{0};
    }

    while (no) {
        digits.emplace_back(no % 10);
        no /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    return digits;
}

std::vector<long> get_digits(long no) {
    if (no == 0)
        return {0};

    std::string num_str = std::to_string(no);
    std::vector<long> digits;
    digits.reserve(num_str.size());

    for (char c : num_str) {
        digits.push_back(c - '0');
    }
    return digits;
}

std::pair<long, long> split_digits(const std::vector<long> &dgs) {
    assert((dgs.size() % 2) == 0);
    assert(dgs.size() >= 2);


    long first = 0, second = 0;
    const auto mid = dgs.size() / 2;
    // std::println("mid @ {}", mid);

    // Calculate first half
    for (size_t i = 0; i < mid; ++i) {
        first = first * 10 + dgs[i];
    }

    // Calculate second half
    for (size_t i = mid; i < dgs.size(); ++i) {
        second = second * 10 + dgs[i];
    }

    return std::make_pair(first, second);
}

#ifdef false
void blink(std::vector<stone> &stones) {
    std::vector<stone> new_stones;
    new_stones.reserve(stones.size() * 2);

    for (auto idx = 0; idx < stones.size(); idx++) {
        const auto &cs = stones[idx];

        if (cs == 0) {
            // stones[idx] = 1;
            new_stones.push_back(1);
            continue;
        }

        const auto dgs = get_digits(cs);

        if ((dgs.size() % 2) == 0) {
            const auto[first, second] = split_digits(dgs);
            // std::println("{} and {}", split.first, split.second);
            // stones.emplace(stones.begin() + idx, split.first);
            // stones[idx + 1] = split.second;
            // idx++;
            new_stones.push_back(first);
            new_stones.push_back(second);
        } else {
            // stones[idx] = stones[idx] * 2024;
            new_stones.push_back(cs * 2024);
        }
    }

    stones = std::move(new_stones);
}
#endif

void print(const std::vector<stone> &stones) {
    for (const auto &stone : stones) {
        std::print("{} ", stone);
    }
    std::println();
}

// <stone value>, <remaining blinks>
using ck = std::pair<stone, int>;
std::unordered_map<ck, long, uwu_hash> sim_cache{};

long simulate_stone_memoized(stone initial, int n_blinks);

long simulate_stone(stone initial, int n_blinks) {
    if (n_blinks == 0) {
        return 1;
    }

    if (initial == 0) {
        return simulate_stone_memoized(1, n_blinks - 1);
    }

    const auto dgs = get_digits(initial);
    if ((dgs.size() % 2) == 0) {
        const auto[first, second] = split_digits(dgs);
        return simulate_stone_memoized(first, n_blinks - 1) +
               simulate_stone_memoized(second, n_blinks - 1);
    } else {
        return simulate_stone_memoized(initial * 2024, n_blinks - 1);
    }
}

long simulate_stone_memoized(stone initial, int n_blinks) {
    const auto key = std::make_pair(initial, n_blinks);
    if (sim_cache.find(key) != sim_cache.end()) {
        return sim_cache[key];
    }
    const auto r = simulate_stone(initial, n_blinks);
    sim_cache[key] = r;
    return r;
}

long fast_blink(const std::vector<stone> &stones, int blinks) {
    long total = 0;
    for (const auto &stone : stones) {
        // std::println("fs for {}", stone);
        total += simulate_stone_memoized(stone, blinks);
    }
    return total;
}



void Day11::run() {
    constexpr auto file_path = "./inputs/day11.input";
    std::ifstream infile(file_path);

    std::vector<stone> stones{};

    long i;
    while (infile >> i) {
        stones.emplace_back(i);
    }

    print(stones);


    constexpr auto bc = 75;
    /*for (auto b = 0; b < bc; b++) {
        std::println("@ blink {}", b);
        blink(stones);
        // print(stones);
        std::println("Stones: {}", stones.size());
    }*/

    auto start = std::chrono::steady_clock::now();
    const auto fb = fast_blink(stones, bc);
    std::cout << "Elapsed(ms)=" << since(start).count() << std::endl;
    std::println("{}", sim_cache.size());
    std::println("Stones: {}", fb);
}
