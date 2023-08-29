#pragma once

#ifndef BRUTE_SOLVER
#define BRUTE_SOLVER

#include <vector>
#include <ctype.h>
#include <unordered_set>

class BruteSolver {
private:
    constexpr static inline int BOARD_SIZE = 9;

    std::vector<std::vector<char>> board_;

    std::vector<std::vector<std::unordered_set<int>>> possible_nums;

    std::unordered_set<int> empty_cells;

    char charFromInt(int x) {
        return (char)(x + '0');
    }
    int intFromChar(char c) {
        return (int)(c - '0');
    }
    int getIndex(int x, int y) {
        return 9 * x + y;
    }
    std::pair<int, int> getCoordinates(int ind) {
        return {ind / 9, ind % 9};
    }

    bool needToSolve() { return !empty_cells.empty(); }

public:
    BruteSolver(std::vector<std::vector<char>> board);

    void usualUpdate(int row, int col);
    void hardUpdate(int row, int col, int previous_cell);

    void solve();
    
    ~BruteSolver() {}
};

#endif