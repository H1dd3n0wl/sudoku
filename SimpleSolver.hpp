#pragma once

#ifndef SIMPLE_SOLVER
#define SIMPLE_SOLVER

#include <vector>
#include <unordered_map>

class SimpleSolver {
private:
    std::vector<std::vector<char>> board_;

    constexpr static inline int BOARD_SIZE = 9;

public:

    SimpleSolver(std::vector<std::vector<char>> board);

    bool isValid();

    ~SimpleSolver() {}

};


#endif