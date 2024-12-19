#include "Day19.h"

#include "../lib.h"
#include <unordered_set>

bool try_make(std::unordered_set<std::string> &can_make, const std::string &target) {
    auto could_replace = false;

    std::queue<std::string> candidates{};
    candidates.emplace(target);

    auto hit = false;

    while (candidates.size() > 0) {
        const auto curr = candidates.front();
        candidates.pop();

        if (can_make.contains(curr)) {
            hit = true;
            break;
        }

        // can_make.emplace(target.substr(0, target.size() - curr.size()));

        if (curr.size() == 0) {
            hit = true;
            break;
        }

        for (const auto &pattern : can_make) {
            if (curr.starts_with(pattern)) {
                candidates.emplace(curr.substr(pattern.size()));
            }
        }
    }

    return hit;
}

bool fast_try_make(std::unordered_set<std::string> &can_make, const std::string &target) {
    auto could_replace = false;

    std::string acc_target = target;

    while (acc_target.size() > 0) {
        could_replace = false;
        for (const auto &pattern : can_make) {
            if (acc_target.starts_with(pattern)) {
                acc_target = acc_target.substr(pattern.size());
                could_replace = true;
            }
        }

        if (!could_replace)
            return false;
    }

    return true;
}

bool wrap_try_make(std::unordered_set<std::string> &can_make, const std::string &target) {
    bool fast_try = fast_try_make(can_make, target);
    if (fast_try)
        return true;
    std::println("fallback to slow try make");
    return try_make(can_make, target);
}

bool pff_dp(std::unordered_set<std::string> &can_make, const std::string &target) {
    std::vector<bool> dp(target.length() + 1, false);
    dp[0] = true;

    for (auto i = 1; i <= target.length(); i++) {
        for (const auto &pattern : can_make) {
            const auto pl = pattern.length();
            if (i >= pl) {
                bool matches = true;
                for (int j = 0; j < pl; j++) {
                    if (pattern[j] != target[i - pl + j]) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    dp[i] = dp[i] || dp[i - pl];
                    if (dp[i])
                        break;
                }
            }
        }
    }

    return dp[target.length()];
}

unsigned long pff_pff_dp(std::unordered_set<std::string> &can_make, const std::string &target) {
    std::vector<unsigned long> dp(target.length() + 1, 0);
    dp[0] = 1;

    for (auto i = 1; i <= target.length(); i++) {
        for (const auto &pattern : can_make) {
            const auto pl = pattern.length();
            if (i >= pl) {
                bool matches = true;
                for (int j = 0; j < pl; j++) {
                    if (pattern[j] != target[i - pl + j]) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    dp[i] += dp[i - pl];
                }
            }
        }
    }

    return dp[target.length()];
}


void Day19::run() {
    constexpr auto file_path = "./inputs/day19.input";
    std::ifstream infile(file_path);

    std::vector<std::string> available_towels{};
    std::vector<std::string> wanted{};

    char ch;
    std::string current;
    while (infile >> std::noskipws >> ch) {
        if (ch == '\n') {
            available_towels.emplace_back(current);
            break;
        }
        if (ch == ' ')
            continue;
        if (ch == ',') {
            available_towels.emplace_back(current);
            current = "";
            continue;
        }
        current = current + ch;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.length() == 0)
            continue;
        wanted.emplace_back(line);
    }

    for (const auto &item : available_towels) {
        std::println("a>{}<", item);
    }
    for (const auto &item : wanted) {
        std::println("w>{}<", item);
    }

    std::println("{} available, last: {}", available_towels.size(), available_towels.back());
    std::println("{} wanted, last: {}", wanted.size(), wanted.back());

    std::unordered_set<std::string> can_make(available_towels.begin(), available_towels.end());

    /*
    std::println("spicing..");
    // spice it up

    for (const auto &cm1 : available_towels) {
        for (const auto &cm2 : available_towels) {
            //for (const auto &cm3 : available_towels) {
            can_make.emplace(cm1 + cm2);
            //}
        }
    }*/

    auto can = 0, cant = 0;

    auto count = 0;
    unsigned long combs = 0;
    for (const auto &w : wanted) {
        if (pff_dp(can_make, w)) {
            can++;
        } else {
            cant++;
        }
        combs += pff_pff_dp(can_make, w);
        // std::println("{} / {}", count++, wanted.size());
    }


    std::println("Can: {}, Can't: {}", can, cant);
    std::println("Pff: {}", combs);
}
