#include "Day16.h"

#include "../lib.h"

void Day16::run() {
    constexpr auto file_path = "./inputs/day16.input";
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
