#include "Day3.h"

#include "../lib.h"

void Day3::run() {
    constexpr auto file_path = "./inputs/day3.input";
    std::ifstream infile(file_path);

    std::vector<std::tuple<int, int>> valid_muls{};

    int mulState = 0;
    int doState = 0;

    std::vector<int> digits{};

    int firstNo = 0;
    int secondNo = 0;

    bool doMul = true;

    std::string line;
    while (std::getline(infile, line)) {
        for (auto idx = 0; idx < line.size(); idx++) {
            const auto ch = line[idx];
            if (ch == 'm' && mulState == 0) {
                mulState = 1;
            } else if (ch == 'u' && mulState == 1) {
                mulState = 2;
            } else if (ch == 'l' && mulState == 2) {
                mulState = 3;
            } else if (ch == '(' && mulState == 3) {
                mulState = 4;
                digits.clear();
            } else if (ch >= '0' && ch <= '9' && (mulState == 4 || mulState == 5)) {
                int n = ch - '0';
                digits.push_back(n);
            } else if (ch == ',' && mulState == 4) {
                mulState = 5;
                firstNo = 0;
                secondNo = 0;
                // std::print("Encountered comma, first digits are: ");
                // std::ranges::copy(digits, std::ostream_iterator<int>(std::cout, " "));
                // std::println();
                for (auto dg = 0; dg < digits.size(); dg++) {
                    int place = digits.size() - dg - 1;

                    int acc = 1;
                    for (int i = 0; i < place; i++) {
                        acc *= 10;
                    }
                    firstNo += (digits[dg] * acc);
                }
                digits.clear();
            } else if (ch == ')' && mulState == 5) {
                for (auto dg = 0; dg < digits.size(); dg++) {
                    int place = digits.size() - dg - 1;

                    int acc = 1;
                    for (int i = 0; i < place; i++) {
                        acc *= 10;
                    }
                    secondNo += (digits[dg] * acc);
                }
                std::println("Encounterd valid mul({},{}) , shouldMul={}", firstNo, secondNo, doMul);
                if (doMul)
                    valid_muls.emplace_back(firstNo, secondNo);

                digits.clear();
                firstNo = 0;
                secondNo = 0;
                mulState = 0;
            } else {
                mulState = 0;
            }

            if (ch == 'd' && doState == 0) {
                doState = 1;
            } else if (ch == 'o' && doState == 1) {
                doState = 2;
            } else if (ch == '(' && doState == 2) {
                doState = 3;
            } else if (ch == ')' && doState == 3) {
                doMul = true;
                doState = 0;
            } else if (ch == 'n' && doState == 2) {
                doState = 10;
            } else if (ch == '\'' && doState == 10) {
                doState = 11;
            } else if (ch == 't' && doState == 11) {
                doState = 12;
            } else if (ch == '(' && doState == 12) {
                doState = 13;
            } else if (ch == ')' && doState == 13) {
                doState = 0;
                doMul = false;
            } else {
                doState = 0;
            }
        }
    }

    const auto mulAcc = std::accumulate(valid_muls.begin(), valid_muls.end(), 0, [](int acc, std::tuple<int, int> par) {
        return acc + (std::get<0>(par) * std::get<1>(par));
    });
    std::println("Accumulated: {}", mulAcc);
}
