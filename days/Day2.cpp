#include "Day2.h"

#include <iostream>

#include "../lib.h"

bool is_safe(std::vector<int> level)
{
    int direction = 0;
    int prev = -1;
    for (const int reading: level) {
        if (prev != -1) {
            int current_dir = (reading > prev) ? 1 : -1;
            if (direction == 0) {
                direction = current_dir;
            }

            if (current_dir != direction) {
                return false;
            }

            if (std::abs(reading - prev) < 1 || std::abs(reading - prev) > 3) {
                return false;
            }

        }

        prev = reading;
    }
    return true;
}


void Day2::run()
{
    constexpr auto file_path = "../inputs/day2.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<int>> reports;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        reports.emplace_back(std::vector<int>(std::istream_iterator<int>(iss),
                                              std::istream_iterator<int>()));
    }

    const auto safe_reports = std::accumulate(reports.begin(), reports.end(), 0, [](const int acc, const std::vector<int> level) {
        const auto safe = is_safe(level);
        // std::ranges::copy(level, std::ostream_iterator<int>(std::cout, " "));
        // std::println("is safe: {}", safe);
        return acc + safe;
    });
    std::println("Safe reports: {}", safe_reports);
}

