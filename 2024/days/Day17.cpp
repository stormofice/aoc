#include "Day17.h"

#include "../lib.h"
#include <ostream>
#include <unordered_map>


int to_value(std::unordered_map<char, unsigned long long> &regs, int operand) {
    if (operand <= 3)
        return operand;
    if (operand >= 7) {
        // std::abort();
    }
    const auto offset = operand - 4;
    return regs[(char)('A' + offset)];
}

struct RState {
    unsigned long long A;
    unsigned long long B;
    unsigned long long C;
};

int step_once(RState &rstate) {
    rstate.B = rstate.A % 8;
    rstate.B ^= 7;
    rstate.C = (rstate.A / (1ULL << rstate.B));
    rstate.B ^= 7;
    rstate.B ^= rstate.C;
    rstate.A /= 8;
    return rstate.B;
}


int step_once(unsigned long long a) {
    unsigned long long b = 0, c = 0;
    b = a % 8;
    b ^= 7;
    c = (a / (1ULL << b));
    b ^= 7;
    b ^= c;
    a /= 8;
    return b % 8;
}

bool simulate_steps(const std::vector<int> &insns, unsigned long long a) {
    auto state = RState{
        .A = a,
        .B = 0,
        .C = 0,
    };

    // std::println("mhm with init a {}", a);
    int outs = 0;
    for (;;) {
        auto ot = step_once(state) % 8;
        // std::print("{},", ot);

        if (insns[outs] != ot)
            return false;

        outs++;
        if (state.A == 0) {
            return true;
        }
    }
    // std::println();
    return true;
}

std::vector<int> perform_run(const std::vector<int> &insns, unsigned long long init_a_val) {
    std::unordered_map<char, unsigned long long> regs{
        {'A', init_a_val},
        {'B', 0},
        {'C', 0},
    };

    auto ip = 0;

    std::vector<int> output{};

    while (ip < insns.size() && ip >= 0) {
        auto cmd = insns[ip];
        auto par = insns[ip + 1];

        auto parV = to_value(regs, par);

        if (cmd == 0) {
            // adiv
            const auto pow = 1 << parV;
            regs['A'] /= pow;
        } else if (cmd == 1) {
            // bxl
            regs['B'] ^= par;
        } else if (cmd == 2) {
            // bst
            regs['B'] = (parV % 8);
        } else if (cmd == 3) {
            // jnz
            if (regs['A'] == 0) {

            } else {
                ip = par;
                continue;
            }
        } else if (cmd == 4) {
            // bxc
            regs['B'] ^= regs['C'];
        } else if (cmd == 5) {
            const auto vv = parV % 8;
            /*if (insns[output.size()] != vv) {
                return {};
            }*/
            output.emplace_back(vv);
        } else if (cmd == 6) {
            // bdv
            const auto pow = 1 << parV;
            regs['B'] = regs['A'] / pow;
        } else if (cmd == 7) {
            // bdv
            const auto pow = 1 << parV;
            regs['C'] = regs['A'] / pow;
        }

        ip += 2;
    }

    return output;
}

#include <bitset>
void print_run(const std::vector<int> &insns, unsigned long long a) {
    std::print("Simulation for rA={}, bin=0b", a);
    // grattlsprache des todes
    std::cout << std::bitset<32>(a) << std::endl;
    const auto run_res = perform_run(insns, a);
    for (const auto &item : run_res) {
        std::print("{},", item);
    }
    std::println();
}

void backwards_cook() {
    const std::vector<int> insns = {2, 4, 1, 7, 7, 5, 1, 7, 4, 6, 0, 3, 5, 5, 3, 0};
    auto state = RState{
        .A = 0,
        .B = 0,
        .C = 0,
    };

    int digit = insns.size() - 1;

    auto ca = 0ULL;

    while (true) {
        for (auto a = ca;; a++) {
            state.A = a;
            state.B = 0;
            state.C = 0;
            auto res = step_once(state);
            if (res == insns[digit]) {
                ca = (a << 3);
                digit--;
                break;
            }
        }
        std::println("a = {}", state.A);
        std::cout << std::bitset<64>(state.A) << std::endl;
    }
}

unsigned long long backtrack(const std::vector<int> &insns, unsigned long long currentA, std::vector<int> &output) {
    if (insns == output) {
        std::println("insns == output? {}", currentA);
        for (const auto &o : output) {
            std::print("{},", o);
        }
        return currentA;
    }

    if (output.size() >= insns.size()) {
        return 0;
    }

    std::println("probing for {} (cA=", insns[output.size()]);
    std::cout << std::bitset<32>(currentA) << ")" << std::endl;
    for (auto i = 0; i <= 7; i++) {

        auto candidate = currentA * 8 + i;
        auto res = step_once(candidate) % 8;

        std::println("{}, insns[output.size()]={}", res, insns[output.size()]);
        if (insns[output.size()] == res) {
            std::println("go");
            output.emplace_back(res);
            auto bt = backtrack(insns, candidate, output);
            if (bt != 0)
                return bt;
            output.pop_back();
        }
    }

    return 0ULL;
}

std::unordered_map<int, std::set<int>> build_lookup_table() {
    std::unordered_map<int, std::set<int>> lookup;
    for (int i = 0; i < 1024; i++) {
        int output = step_once(i);
        lookup[output].insert(i);
    }
    return lookup;
}

long long hope(std::vector<int> &insns, std::unordered_map<int, std::set<int>> &lut, int pos, long long rem) {
    pos--;
    if (pos < 0)
        return rem;

    rem <<= 3;
    for (const auto &bits : lut[insns[pos]]) {
        if ((bits & 0b1111111000) == (rem & 0b1111111000)) {
            const auto bt = hope(insns, lut, pos, rem | bits);
            if (bt > -1)
                return bt;
        }
    }
    return -1;
}

#include <atomic>
#include <thread>
void Day17::run() {
    // backwards_cook();
    // return;
    std::vector<int> insns = {2, 4, 1, 7, 7, 5, 1, 7, 4, 6, 0, 3, 5, 5, 3, 0};

    auto look = build_lookup_table();
    const auto hp = hope(insns, look, insns.size(), 0);
    std::println("{}", hp);
    print_run(insns, hp);
    /*

    for (const auto & [ k, v ] : look) {
        std::print("{}: ", k);
        for (const auto &item : v) {
            std::print("{}, ", item);
        }
        std::println();
    }

    auto ccA = 0ULL;

    int iWant = 0;
*/

    return;
    // std::cout << std::bitset<32>(0b010 << 3) << std::endl;
    // std::cout << std::bitset<32>((0b010 << 3) | 0b111) << std::endl;

    std::vector<int> out{};
    const auto bt = backtrack(insns, 0ULL, out);
    std::println("backtrack: {}", bt);

    std::println("ver:");
    print_run(insns, bt);

    return;
    const unsigned long long mm = (1ULL << 48);

    /*
    print_run(insns, (1ULL << 46));
    print_run(insns, (1ULL << 47));
    print_run(insns, (1ULL << 48));
    print_run(insns, (1ULL << 49));
    print_run(insns, (1ULL << 50));
    print_run(insns, (1ULL << 51));*/

    const auto xxd = 35184372088832ULL;
    const auto ppx = 281474976710656ULL;
    const int num_threads = 16;
    std::atomic<bool> found{false};
    std::atomic<unsigned long long> result{0};

    const unsigned long long chunk_size = (ppx - xxd) / num_threads;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        unsigned long long start = xxd + (i * chunk_size);
        unsigned long long end = (i == num_threads - 1) ? ppx : start + chunk_size;

        threads.emplace_back([start, end, &insns, &found, &result]() {
            for (unsigned long long id = start; id < end && !found; id++) {
                if (simulate_steps(insns, id)) {
                    result = id;
                    found = true;
                    break;
                }
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    if (found) {
        std::println("Found quine with init a={}", result.load());
    } else {
        std::println("nop");
    }

    return;
    /*
    const auto xxd = 35184372088832ULL;
    const auto ppx = 281474976710656ULL;

    for (unsigned long long id = xxd; id <= ppx; id++) {
        const auto srs = simulate_steps(insns, id);
        if (srs) {
            std::println("Found quine with init a={}", id);
            break;
        }
    }*/

    // 28
    // 0010 1000

    print_run(insns, 47237);

    // print_run(insns, 0b000);
    print_run(insns, 0b000'000'000);
    print_run(insns, 0b000'000'001);
    print_run(insns, 0b000'001'001);
    print_run(insns, 0b001'001'001);

    print_run(insns, 0b100'101'101);
    print_run(insns, 0b001'101'101);

    print_run(insns, 0b001'101'101'001'101'101);
    print_run(insns, 0b101'101'101'001'101'101);
    print_run(insns, 0b001'101'101'101'101'101);



    return;
    print_run(insns, 0b001'001'001'001);
    print_run(insns, 0b001'001'001'001'001);
    print_run(insns, 0b001'000'000'000'000);





    return;
    for (auto i = 0; i <= 15; i++) {
        print_run(insns, i);
    }

    for (auto i = 16; i <= 31; i++) {
        print_run(insns, i);
    }


    return;


    return;
    const auto top = 1ULL << 48;

    for (auto i = 0ULL; i < top; i++) {

        const auto run_res = perform_run(insns, i);
        if (run_res == insns) {
            std::println("Lowest A: {}", i);
            break;
        }
    }

    std::println("Fin.");
}
