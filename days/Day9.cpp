#include "Day9.h"

#include "../lib.h"

void print_disk(const std::vector<int> &disk) {
    for (const auto &item : disk) {
        if (item == -1) {
            std::print(".");
        } else {
            std::print("{}", item);
        }
    }
    std::println();
}

constexpr bool debug = true;

void defrag_disk(std::vector<int> &disk) {

    int last_file_idx = disk.size() - 1;

    for (auto idx = 0; idx < last_file_idx; idx++) {
        const auto ch = disk[idx];
        if (ch == -1) {
            for (auto back = last_file_idx; back > idx; back--) {
                if (disk[back] != -1) {
                    if (debug)
                        std::println("Moving disk[{}] ({}) to disk[{}]", back, disk[back], idx);
                    disk[idx] = disk[back];
                    disk[back] = -1;
                    if (debug)
                        print_disk(disk);
                    last_file_idx = back - 1;
                    break;
                }
            }
        }

        // std::cin.get();
    }
}

void whole_file_defrag(std::vector<int> &disk) {

    int fsize = 0;


    std::unordered_set<int> moved{};

    for (auto back = disk.size() - 1; back > 1; back--) {
        auto ch = disk[back];

        if (ch == -1) {
            fsize = 0;
            continue;
        }
        fsize++;
        auto look_ahead = disk[back - 1];

        if (ch == look_ahead) {
            continue;
        }

        // std::println("Now looking to move a file of {} w/ size {}", ch, fsize);

        if (!moved.insert(ch).second) {
            // std::println("Already moved");
            fsize = 0;
            continue;
        }

        int cur_fs_l = 0;
        int free_space_start = -1;

        for (auto f = 0; f < disk.size() && f < back; f++) {
            if (disk[f] != -1) {
                cur_fs_l = 0;
                free_space_start = -1;
                continue;
            }
            if (free_space_start == -1) {
                free_space_start = f;
            }
            cur_fs_l++;

            if (cur_fs_l == fsize) {
                for (auto m = 0; m < fsize; m++) {
                    disk[free_space_start + m] = disk[back + m];
                    disk[back + m] = -1;
                }
                // print_disk(disk);
                break;
            }
        }

        fsize = 0;
    }
}

long calculate_checksum(const std::vector<int> &disk) {
    long checksum = 0;

    int idx = 0;
    for (const auto &item : disk) {

        if (item == -1) {

        } else {
            // std::println("+= {} * {}", item, idx);
            checksum += (item * idx);
        }
        idx++;
    }

    return checksum;
}

void Day9::run() {
    constexpr auto file_path = "./inputs/day9.input";
    std::ifstream infile(file_path);


    std::vector<int> disk{};

    char ch;
    bool is_file = true;
    int file_id = 0;
    while (infile >> std::noskipws >> ch) {
        int no = ch - '0';

        for (auto i = 0; i < no; i++) {
            if (is_file) {
                disk.emplace_back(file_id);
            } else {
                disk.emplace_back(-1);
            }
        }

        if (is_file) {
            file_id++;
        }

        is_file = !is_file;
    }

    if (debug)
        print_disk(disk);

    // defrag_disk(disk);

    whole_file_defrag(disk);

    const auto chk = calculate_checksum(disk);
    std::println("Checksum of defragmented disk: {}", chk);
}
