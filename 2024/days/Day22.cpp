#include "Day22.h"

#include "../lib.h"
#include <numeric>

long long evolve(long long number) {
    auto r = number * 64;
    number ^= r;
    number %= 16777216;

    auto d = number / 32;
    number ^= d;
    number %= 16777216;

    auto lm = number * 2048;
    number ^= lm;
    number %= 16777216;

    return number;
}

long long qhash(const std::vector<long long> &deltas) {
    long long acc = 0;
    for (int i = 0; i < 4; i++) {
        // Shift each number up by 10 to make it non-negative (0-20)
        // Then pack it into its own 5-bit section
        acc = (acc << 5) | (deltas[i] + 10);
    }
    return acc;
}

std::vector<long long> unhash(long long hash) {
    std::vector<long long> result;
    for (int i = 0; i < 4; i++) {
        // Extract 5 bits and convert back to original range
        result.insert(result.begin(), (hash & 0x1F) - 10);
        hash >>= 5;
    }
    return result;
}

void print_unhashed(long long hash) {
    const auto uh = unhash(hash);
    for (const auto &i : uh) {
        std::print("{} ", i);
    }
}

long long search_for_sequence(const std::vector<std::unordered_map<long long, long long>> &sequences) {
    std::unordered_map<long long, long long> hash_prices{};

    for (const auto &seq_map : sequences) {
        for (const auto & [ hash, price ] : seq_map) {

            // search all sequence maps for this hash
            if (hash_prices.contains(hash))
                continue;
            hash_prices.emplace(hash, price);

            for (const auto &search_map : sequences) {
                if (search_map == seq_map)
                    continue;

                if (search_map.contains(hash)) {
                    hash_prices[hash] += search_map.at(hash);
                }
            }
        }

        std::print(".");
    }
    std::println();

    auto maxV = 0ULL;
    auto maxH = 0;


    for (const auto & [ hash, price ] : hash_prices) {
        print_unhashed(hash);
        std::println(": {}", price);
        if (price > maxV) {
            maxV = price;
            maxH = hash;
        }
    }

    print_unhashed(maxH);
    std::println("produced max: {}", maxV);

    return 0;
}

void pc(const std::vector<long long> &c) {
    for (const auto &item : c) {
        std::print("{}, ", item);
    }
    std::println();
}

void Day22::run() {
    constexpr auto file_path = "./inputs/day22.input";
    std::ifstream infile(file_path);

    std::vector<long long> nums{};
    long long n;
    while (infile >> n) {
        // std::println("[in] {}", n);
        nums.emplace_back(n);
    }
    // std::vector<long long> nums{1, 2, 3, 2024};

    constexpr auto step_count = 2000;

    std::vector<long long> results{};

    std::vector<std::unordered_map<long long, long long>> sequences{};

    for (auto num : nums) {
        std::unordered_map<long long, long long> hashtrack{};

        const auto sv = num;
        long long prev = 0;

        std::vector<long long> changes{};
        for (auto i = 0; i < step_count; i++) {
            auto current_price = num % 10;
            num = evolve(num);
            auto delta = current_price - prev;
            prev = current_price;
            if (changes.size() == 4) {
                changes.erase(changes.begin());
            }
            changes.emplace_back(delta);
            if (changes.size() < 4)
                continue;
            // pc(changes);
            const auto qq = qhash(changes);
            if (hashtrack.contains(qq))
                continue;
            hashtrack.emplace(qq, current_price);
        }
        // std::println("{} produced {} after {} steps", sv, num, step_count);
        sequences.emplace_back(hashtrack);

        results.emplace_back(num);
    }

    const auto res = std::accumulate(results.begin(), results.end(), 0ULL, [](long long acc, long long n) { return acc + n; });
    std::println("Checksum: {}", res);

    search_for_sequence(sequences);
}
