#include "Day23.h"

#include "../lib.h"

void Day23::run() {
    constexpr auto file_path = "./inputs/day23.input";
    std::ifstream infile(file_path);

    std::unordered_map<int, std::unordered_set<int>> graph{};
    std::unordered_map<std::string, int> idx_map{};
    std::unordered_map<int, std::string> str_map{};

    char ch;
    std::string first = "";
    std::string second = "";
    std::string *current = &first;

    while (infile >> std::noskipws >> ch) {
        if (ch == '-') {
            if (!idx_map.contains(first)) {
                auto new_idx = idx_map.size();
                idx_map[first] = new_idx;
                str_map[new_idx] = first;
            }
            current = &second;
            continue;
        }

        if (ch == '\n') {
            if (!idx_map.contains(second)) {
                const auto new_idx = idx_map.size();
                idx_map[second] = new_idx;
                str_map[new_idx] = second;
            }

            const int idx1 = idx_map[first];
            const int idx2 = idx_map[second];

            if (!graph.contains(idx1)) {
                graph[idx1] = std::unordered_set<int>{};
            }
            graph[idx1].insert(idx2);

            if (!graph.contains(idx2)) {
                graph[idx2] = std::unordered_set<int>{};
            }
            graph[idx2].insert(idx1);

            first = "";
            second = "";
            current = &first;
            continue;
        }

        *current += ch;
    }

    if (!first.empty() && !second.empty()) {
        int idx1 = idx_map[first];
        int idx2 = idx_map[second];
        if (!graph.contains(idx1))
            graph[idx1] = std::unordered_set<int>{};
        if (!graph.contains(idx2))
            graph[idx2] = std::unordered_set<int>{};
        graph[idx1].insert(idx2);
        graph[idx2].insert(idx1);
    }

    std::println("{} nodes (verify: {})", idx_map.size(), graph.size());
    for (const auto & [ node, neighbors ] : graph) {
        std::print("{}: ", str_map[node]);
        for (const auto &item : neighbors) {
            std::print("{}, ", str_map[item]);
        }
        std::println();
    }


    auto cq = 0;

    // 3 cliques
    for (const auto & [ n1, nn1 ] : graph) {
        for (const auto & [ n2, nn2 ] : graph) {
            if (n2 <= n1)
                continue;

            if (!nn1.contains(n2))
                continue;

            for (const auto & [ n3, nn3 ] : graph) {
                if (n3 <= n2)
                    continue;

                if (nn1.contains(n3) && nn2.contains(n3)) {
                    if (str_map[n1].starts_with('t') || str_map[n2].starts_with('t') || str_map[n3].starts_with('t'))
                        cq++;
                }
            }
        }
    }

    std::println("{} of 3-cliques start with t", cq);
}
