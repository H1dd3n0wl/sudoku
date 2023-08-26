#pragma once

#ifndef BRUTE_SOLVER
#define BRUTE_SOLVER

#include <vector>

class BruteSolver {
private:
    constexpr static inline int BOARD_SIZE = 9;

    std::vector<std::vector<char>> board_;

public:
    BruteSolver(std::vector<std::vector<char>> board);
    
    ~BruteSolver() {}
};

#endif