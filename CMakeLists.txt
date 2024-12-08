cmake_minimum_required(VERSION 3.30)
project(aoc2024)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -g
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Werror # Treat warnings as errors
    -Wno-unused-variable
    -Wshadow # Warn if a variable declaration shadows one from a parent context
    -Wformat=2 # Perform additional format string checks
    -Wcast-align # Warn about pointer casts that increase alignment
    -Wconversion # Warn for implicit conversions that may change the value2
  #  -Wsign-conversion # Warn for implicit conversions between signed/unsigned
    -Wno-sign-compare
    -Wno-conversion
    -Wno-shadow
    -Wnull-dereference # Warn if a null dereference is detected
    -Wduplicated-cond # Warn if if / else chain has duplicated conditions
    -Wduplicated-branches # Warn if if / else branches have duplicated code
    -Wlogical-op # Warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast # Warn if you perform a cast to the same type
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -O0
        -fno-omit-frame-pointer
    )
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in release mode")
    add_compile_options(
        -Ofast
        -march=native
    )
endif()

add_executable(aoc2024 main.cpp
        days/Day1.cpp
        days/Day1.h
        lib.h
        days/Day2.cpp
        days/Day2.h
        days/Day3.cpp
        days/Day3.h
        days/Day4.cpp
        days/Day4.h
        days/Day5.cpp
        days/Day5.h
        days/Day6.cpp
        days/Day6.h
        days/Day7.cpp
        days/Day7.h
        days/Day8.cpp
        days/Day8.h
        )
