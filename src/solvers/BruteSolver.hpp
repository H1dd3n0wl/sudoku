#pragma once

#ifndef BRUTE_SOLVER
#define BRUTE_SOLVER

#include <vector>
#include <ctype.h>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iostream>

class BruteSolver {
private:
    constexpr static inline int BOARD_SIZE = 9;

    std::vector<std::vector<char>> board_;

    std::vector<std::vector<std::unordered_set<int>>> possible_nums;

    std::unordered_set<int> empty_cells;

    std::unordered_map<int, std::vector<std::pair<int, int>>> updated_cells; 
    std::vector<int> cells_stack;

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

    void resetChange(int row, int col);

public:
    BruteSolver(std::vector<std::vector<char>> board);
    BruteSolver(std::string file);

    void simpleUpdate(int row, int col);
    bool update(int row, int col);
    bool hardUpdate(int row, int col);

    void solve();

    void printBoard();
    
    ~BruteSolver() {}
};

#endif