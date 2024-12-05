#include "Day1.h"

#include "../lib.h"

void Day1::run() {
    constexpr auto file_path = "../inputs/day1.input";
    std::ifstream infile(file_path);

    std::vector<int> first{};
    std::vector<int> second{};

    int a, b;
    while (infile >> a >> b) {
        first.push_back(a);
        second.push_back(b);
    }

    std::println("Vectors: first = {}, second = {}", first.size(), second.size());

    assert(first.size() == second.size());

    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());

    int total_distance = 0;

    for (auto idx = 0; idx < first.size(); idx++) {
        total_distance += std::abs(first[idx] - second[idx]);
    }

    std::println("Total distance: {}", total_distance);

    std::unordered_map<int, int> right{};
    std::for_each(second.begin(), second.end(), [&right](const int x) {
        if (const auto it = right.find(x); it == right.end()) {
            right[x] = 1;
        } else {
            it->second++;
        }
    });

    const auto similarity = std::accumulate(first.begin(), first.end(), 0, [&right](int acc, int x) {
        if (const auto match = right.find(x); match != right.end()) {
            return acc + (x * match->second);
        }
        return acc;
    });

    std::println("Total similarity: {}", similarity);
}
