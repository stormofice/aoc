#include "Day20.h"

#include "../lib.h"

void Day20::run() {
    constexpr auto file_path = "./inputs/day20.input";
    std::ifstream infile(file_path);

    char ch;
    auto idx = 0;
    while (infile >> std::noskipws >> ch) {
        if (ch == '\n') {
            idx++;
            continue;
        }
    }
}
