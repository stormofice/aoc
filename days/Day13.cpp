#include "Day13.h"
#include <ortools/linear_solver/linear_solver.h>

#include "../lib.h"

using namespace operations_research;
using nt = unsigned long long;
using point = std::pair<nt, nt>;

struct Vending {
    point location;
    point a, b;
};

void Day13::run() {
    constexpr auto file_path = "./inputs/day13.input";
    std::ifstream infile(file_path);

    std::vector<Vending> machines{};
    std::string line, label;
    char delimiter;

    Vending current;
    while (std::getline(infile, line)) {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string fw, sw;
        char x_char, eq_or_p, y_char, comma;
        nt x, y;

        iss >> fw;
        if (fw == "Button") {
            iss >> sw;
            iss >> x_char >> eq_or_p >> x;
            iss >> comma >> y_char >> eq_or_p >> y;

            if (sw == "A:") {
                current.a = {x, y};
            } else if (sw == "B:") {
                current.b = {x, y};
            }
        } else if (fw == "Prize:") {
            iss >> x_char >> eq_or_p >> x;
            iss >> comma >> y_char >> eq_or_p >> y;
            x += 10000000000000ULL;
            y += 10000000000000ULL;
            current.location = {x, y};
            machines.push_back(current);
        }
    }

    nt sum = 0;
    for (size_t i = 0; i < machines.size(); ++i) {
        std::println("Machine {}:", i + 1);
        const auto &v = machines[i];

        // 😎
        auto solver = MPSolver::CreateSolver("SCIP");
        if (!solver) {
            std::abort();
        }

        // insanely large number for part2
        auto N = solver->MakeIntVar(0, 1e16, "N");
        auto M = solver->MakeIntVar(0, 1e16, "M");

        if (!N || !M) {
            std::abort();
        }

        auto objective = solver->MutableObjective();
        // button costs
        objective->SetCoefficient(N, 3);
        objective->SetCoefficient(M, 1);
        objective->SetMinimization();

        auto x_constraint = solver->MakeRowConstraint(v.location.first, v.location.first, "x_constraint");
        x_constraint->SetCoefficient(N, v.a.first);
        x_constraint->SetCoefficient(M, v.b.first);

        auto y_constraint = solver->MakeRowConstraint(v.location.second, v.location.second, "y_constraint");
        y_constraint->SetCoefficient(N, v.a.second);
        y_constraint->SetCoefficient(M, v.b.second);

        // N + M <= 100 (pt1)
        // auto sum_constraint = solver->MakeRowConstraint(0, 100, "sum_constraint");
        // sum_constraint->SetCoefficient(N, 1);
        // sum_constraint->SetCoefficient(M, 1);


        const MPSolver::ResultStatus result = solver->Solve();
        if (result == MPSolver::OPTIMAL) {
            const nt n = N->solution_value();
            const nt m = M->solution_value();
            const nt cost = ((nt)3 * n + m);
            std::println("Solution found: N={}, M={}  -- costs {}", n, m, cost);

            // verify solution
            nt x_check = n * v.a.first + m * v.b.first;
            nt y_check = n * v.a.second + m * v.b.second;
            if (x_check != v.location.first || y_check != v.location.second) {
                std::println("Wrong solution?! xd");
                std::println("Expected: ({}, {})", v.location.first, v.location.second);
                std::println("Got: ({}, {})", x_check, y_check);
            } else {
                sum += cost;
            }
        } else {
            std::println("No solution found");
        }
    }

    std::println("sum {}", sum);
}
