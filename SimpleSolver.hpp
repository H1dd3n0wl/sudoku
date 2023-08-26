#pragma once

#ifndef SIMPLE_SOLVER
#define SIMPLE_SOLVER

#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

class SimpleSolver {
private:
    constexpr static inline int BOARD_SIZE = 9;

    std::vector<std::vector<char>> board_;

    std::vector<std::vector<std::set<int>>> possible_numbers;

public:

    SimpleSolver(std::vector<std::vector<char>> board);

    void prepare();

    void update(int row, int col);

    void solve();

    bool isValid();

    std::vector<std::vector<char>> getBoard();

    friend std::ostream& operator<<(std::ostream& p, const SimpleSolver& solver);

    ~SimpleSolver() {}

};


#endif