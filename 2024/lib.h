#pragma once

#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>
#include <print>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iterator>
#include <iostream>
#include <tuple>
#include <ios>
#include <ostream>
#include <utility>
#include <ranges>
#include <chrono>
#include <set>
#include <queue>

template <
    class result_t = std::chrono::milliseconds,
    class clock_t = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds>
auto since(std::chrono::time_point<clock_t, duration_t> const &start) {
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}