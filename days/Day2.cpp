#include "Day2.h"

#include "../lib.h"

bool is_safe(const std::vector<int> &level) {
    int direction = 0;
    int prev = -1;
    for (const int reading : level) {
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


void Day2::run() {
    constexpr auto file_path = "./inputs/day2.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<int>> reports;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        reports.emplace_back(std::istream_iterator<int>(iss),
                             std::istream_iterator<int>());
    }

    auto error_corrected_safe = 0;

    const auto safe_reports = std::accumulate(reports.begin(), reports.end(), 0, [&error_corrected_safe](const int acc, const std::vector<int> level) {
        const auto safe = is_safe(level);
        // std::ranges::copy(level, std::ostream_iterator<int>(std::cout, " "));
        // std::println("is safe: {}", safe);

        if (!safe) {
            for (auto idx = 0; idx < level.size(); idx++) {
                auto rr = level;
                rr.erase(rr.begin() + idx);
                // std::ranges::copy(rr, std::ostream_iterator<int>(std::cout, " "));
                const auto rrsafe = is_safe(rr);
                // std::println("is safe: {}", rrsafe);
                if (rrsafe) {
                    error_corrected_safe++;
                    break;
                }
            }
        }

        return acc + safe;
    });
    std::println("Safe reports: {}", safe_reports);
    std::println("Error corrected reports: {}", error_corrected_safe);
    std::println("Total safe: {}", safe_reports + error_corrected_safe);
}
