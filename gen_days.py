DAY_HEADER_TEMPLATE = r"""#pragma once

class DayÄÄ {
public:
    void run();
};
"""

DAY_CPP_TEMPLATE = r"""#include "DayÄÄ.h"

#include "../lib.h"

void DayÄÄ::run() {
    constexpr auto file_path = "./inputs/dayÄÄ.input";
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
"""


def main():
    main_headers = ""
    cmake_lsts = ""

    for i in range(13, 26):
        hd = DAY_HEADER_TEMPLATE.replace("ÄÄ", str(i))
        cd = DAY_CPP_TEMPLATE.replace("ÄÄ", str(i))

        with open(f"./days/Day{i}.h", "w") as f:
            f.write(hd)

        with open(f"./days/Day{i}.cpp", "w") as f:
            f.write(cd)

        # For main.cpp

        main_headers += f"#include \"days/Day{i}.h\"\n"
        cmake_lsts += f"        days/Day{i}.cpp\n        days/Day{i}.h\n"

    print(main_headers)
    print(cmake_lsts)


if __name__ == "__main__":
    main()
