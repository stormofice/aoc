#include "Day7.h"

#include "../lib.h"

using eq = std::pair<long, std::vector<long>>;

std::vector<std::vector<char>> gen_combinations(const size_t ops) {
    std::vector<std::vector<char>> res{};

    long total_combs = 1;
    for (auto i = 0; i < ops; i++) {
        total_combs *= 3;
    }

    for (auto i = 0; i < total_combs; i++) {
        std::vector<char> current(ops, '+');

        long num = i;
        for (auto k = 0; k < ops; k++) {

            switch (num % 3) {
            case 0:
                break;
            case 1:
                current[k] = '*';
                break;
            case 2:
                current[k] = '|';
                break;
            }
            num /= 3;
        }

        res.emplace_back(current);
    }

    return res;
}

bool is_eq_solvable(const eq &e, std::unordered_map<long, std::vector<std::vector<char>>> comb_cache) {
    const auto result = e.first;
    if (comb_cache.find(e.second.size()) == comb_cache.end()) {
        comb_cache.emplace(e.second.size(), gen_combinations(e.second.size()));
    }
    const auto combs = comb_cache[e.second.size()];

    for (const auto &comb : combs) {

        auto ctry = e.second[0];

        for (auto i = 1; i < e.second.size(); i++) {
            const auto op = comb[i - 1];
            if (op == '+') {
                ctry += e.second[i];
            } else if (op == '*') {
                ctry *= e.second[i];
            } else if (op == '|') {
                std::string lol;
                lol += std::to_string(ctry);
                lol += std::to_string(e.second[i]);
                ctry = std::stol(lol);
            } else {
                std::abort();
            }
        }

        if (ctry == result)
            return true;
    }

    return false;
}

void Day7::run() {
    constexpr auto file_path = "./inputs/day7.input";
    std::ifstream infile(file_path);

    // result: <inputs>
    std::vector<eq> eqs{};

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string part;

        if (std::getline(iss, part, ':')) {
            long key = std::stol(part);
            std::vector<long> values;

            while (iss >> part) {
                values.push_back(std::stol(part));
            }

            eqs.emplace_back(key, values);
        }
    }

    long s_eq_c = 0;

    std::unordered_map<long, std::vector<std::vector<char>>> comb_cache{};

    for (const auto &pair : eqs) {
        auto solvable = is_eq_solvable(pair, comb_cache);
        s_eq_c += solvable ? pair.first : 0;
    }

    /*
    for (const auto &pair : eqs) {
        std::cout << pair.first << ": ";
        for (int value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;

        auto solv = is_eq_solvable(pair);
        std::println("  is {} solvable", solv ? "" : "not");

        s_eq_c += solv ? 1 : 0;
    }*/

    std::println("Solvable equations: {}", s_eq_c);
}
