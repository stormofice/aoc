#include "Day5.h"

#include "../lib.h"

bool is_update_correct(std::unordered_map<int, std::unordered_set<int>> &rules, const std::vector<int> &update) {
    for (auto idx = 0; idx < update.size(); idx++) {
        const auto cur = update[idx];
        if (rules.find(cur) != rules.end()) {
            const auto &not_before = rules[cur];
            for (auto bef = 0; bef < idx; bef++) {
                const auto before = update[bef];
                if (not_before.find(before) != not_before.end()) {
                    return false;
                }
            }
        }
    }

    return true;
}

std::vector<int> order_incorrect(std::unordered_map<int, std::unordered_set<int>> &rules, std::vector<int> &update) {
    std::vector<int> fixed = update;

    /*
    std::print("before: ");
    std::ranges::copy(update, std::ostream_iterator<int>(std::cout, " "));
    std::println();*/

    std::sort(fixed.begin(), fixed.end(), [&rules, &update](int a, int b) {

        auto acount = 0;
        auto bcount = 0;
        for (auto &item : update) {
            if (rules[a].contains(item)) {
                acount++;
            }
            if (rules[b].contains(item)) {
                bcount++;
            }
        }

        return acount > bcount;
    });

    /*
    std::print("fixed: ");
    std::ranges::copy(fixed, std::ostream_iterator<int>(std::cout, " "));
    std::println();*/

    return fixed;
}

void Day5::run() {
    constexpr auto file_path = "./inputs/day5.input";
    std::ifstream infile(file_path);

    std::unordered_map<int, std::unordered_set<int>> rules{};

    std::vector<std::vector<int>> updates{};

    bool vecRead = false;

    std::string line;
    while (std::getline(infile, line)) {

        if (line.empty()) {
            vecRead = true;
            continue;
        }

        if (!vecRead) {
            std::istringstream iss(line);

            std::string token;

            int key;
            int val;
            if (std::getline(iss, token, '|')) {
                key = std::stoi(token);
            }

            // Get the second number
            if (std::getline(iss, token, '|')) {
                val = std::stoi(token);
            }

            const auto nel = rules.emplace(key, std::unordered_set<int>{});
            auto &keyset = nel.first->second;
            keyset.insert(val);
        } else {
            std::vector<int> vec;
            std::stringstream ss(line);
            std::string number;

            while (std::getline(ss, number, ',')) {
                vec.push_back(std::stoi(number));
            }

            updates.push_back(vec);
        }
    }

    std::println("Got {} rule keys and {} updates", rules.size(), updates.size());
    for (const auto & [ key, rset ] : rules) {
        std::print("{}: ", key);
        for (const auto &val : rset) {
            std::print("{} ", val);
        }
        std::println();
    }

    auto middle_sum = 0;
    auto fixed_middle_sum = 0;

    for (auto &item : updates) {
        const auto safe = is_update_correct(rules, item);
        // std::println("Safe: {}", safe);
        if (safe) {
            assert((item.size() % 2) == 1);
            const auto &mitem = item[(item.size() / 2)];
            // std::println("+= {}", mitem);
            middle_sum += mitem;
        } else {
            const auto fixed = order_incorrect(rules, item);
            const auto &mitem = fixed[(fixed.size() / 2)];
            fixed_middle_sum += mitem;
        }
    }
    std::println("Sum of elements (in the middle): {}", middle_sum);
    std::println("Sum of fixed elements (in the middle): {}", fixed_middle_sum);
}
