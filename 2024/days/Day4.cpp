#include "Day4.h"

#include "../lib.h"



int xmas_count(const int &x, const int &y, const std::vector<std::vector<char>> &matrix, const std::vector<char> &line) {
    const auto ch = matrix[y][x];
    if (ch != 'X')
        return 0;

    int count = 0;

    // Horizontal
    if ((x - 3) >= 0) {
        if (matrix[y][x - 1] == 'M' && matrix[y][x - 2] == 'A' && matrix[y][x - 3] == 'S') {
            count++;
        }
    }
    if ((x + 3) < line.size()) {
        if (matrix[y][x + 1] == 'M' && matrix[y][x + 2] == 'A' && matrix[y][x + 3] == 'S') {
            count++;
        }
    }

    // Vertical
    if ((y - 3) >= 0) {
        if (matrix[y - 1][x] == 'M' && matrix[y - 2][x] == 'A' && matrix[y - 3][x] == 'S') {
            count++;
        }
    }
    if ((y + 3) < matrix.size()) {
        if (matrix[y + 1][x] == 'M' && matrix[y + 2][x] == 'A' && matrix[y + 3][x] == 'S') {
            count++;
        }
    }

    // Diagonal left
    if ((x - 3) >= 0) {
        if ((y - 3) >= 0) {
            if (matrix[y - 1][x - 1] == 'M' && matrix[y - 2][x - 2] == 'A' && matrix[y - 3][x - 3] == 'S') {
                count++;
            }
        }
        if ((y + 3) < matrix.size()) {
            if (matrix[y + 1][x - 1] == 'M' && matrix[y + 2][x - 2] == 'A' && matrix[y + 3][x - 3] == 'S') {
                count++;
            }
        }
    }

    // Diagonal right
    if ((x + 3) < line.size()) {
        if ((y - 3) >= 0) {
            if (matrix[y - 1][x + 1] == 'M' && matrix[y - 2][x + 2] == 'A' && matrix[y - 3][x + 3] == 'S') {
                count++;
            }
        }
        if ((y + 3) < matrix.size()) {
            if (matrix[y + 1][x + 1] == 'M' && matrix[y + 2][x + 2] == 'A' && matrix[y + 3][x + 3] == 'S') {
                count++;
            }
        }
    }

    return count;
}


int xmas_count2(const int &x, const int &y, const std::vector<std::vector<char>> &matrix, const std::vector<char> &line) {
    const auto ch = matrix[y][x];
    if (ch != 'A')
        return 0;

    if (y == 0 || y == matrix.size() - 1)
        return 0;
    if (x == 0 || x == line.size() - 1)
        return 0;

    int count = 0;
    const auto top_left = matrix[y - 1][x - 1];
    const auto top_right = matrix[y - 1][x + 1];
    const auto bottom_left = matrix[y + 1][x - 1];
    const auto bottom_right = matrix[y + 1][x + 1];

    int tl_br = 0;
    int tr_bl = 0;

    if (top_left == 'M' && bottom_right == 'S')
        tl_br++;
    if (top_left == 'S' && bottom_right == 'M')
        tl_br++;
    if (top_right == 'M' && bottom_left == 'S')
        tr_bl++;
    if (top_right == 'S' && bottom_left == 'M')
        tr_bl++;
    return (tl_br == 1 && tr_bl == 1) ? 1 : 0;
}

void Day4::run() {
    constexpr auto file_path = "./inputs/day4.input";
    std::ifstream infile(file_path);

    std::vector<std::vector<char>> matrix{};
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<char> line_chars{};
        for (const auto &item : line) {
            line_chars.push_back(item);
        }
        matrix.emplace_back(line_chars);
    }

    auto xmas = 0;
    auto xmas2 = 0;

    for (auto line_idx = 0; line_idx < matrix.size(); line_idx++) {
        const auto line = matrix[line_idx];
        for (auto char_idx = 0; char_idx < line.size(); char_idx++) {
            xmas += xmas_count(char_idx, line_idx, matrix, line);
            xmas2 += xmas_count2(char_idx, line_idx, matrix, line);
        }
    }

    std::println("Found \"XMAS\" {} times", xmas);
    std::println("Found \"X-MAS\" {} times", xmas2);
}
