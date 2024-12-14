#include "Day23.h"

#include "../lib.h"

void Day23::run() {
    constexpr auto file_path = "./inputs/day23.input";
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
