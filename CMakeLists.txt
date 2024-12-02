cmake_minimum_required(VERSION 3.30)
project(aoc2024)

set(CMAKE_CXX_STANDARD 23)

add_executable(aoc2024 main.cpp
        days/Day1.cpp
        days/Day1.h
        lib.h
        days/Day2.cpp
        days/Day2.h)
