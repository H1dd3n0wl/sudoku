#include "SimpleSolver.hpp"

SimpleSolver::SimpleSolver(std::vector<std::vector<char>> board)
    : board_{board},
      possible_numbers(BOARD_SIZE, std::vector<std::set<int>>(
                                       BOARD_SIZE, {1, 2, 3, 4, 5, 6, 7, 8, 9})) {}

bool SimpleSolver::isValid() {
    std::vector<int> column_check(BOARD_SIZE, 0);
    std::vector<int> row_check(BOARD_SIZE, 0);

    auto get([](char c) { return (int)(c - '1'); });

    for (int i = 0; i < BOARD_SIZE; ++i) {
        column_check.assign(BOARD_SIZE, 0);
        row_check.assign(BOARD_SIZE, 0);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board_[i][j] != '.' && row_check[get(board_[i][j])]++) {
                return false;
            }
            if (board_[j][i] != '.' && column_check[get(board_[j][i])]++) {
                return false;
            }
        }
    }

    std::vector<int> block_check(BOARD_SIZE, 0);
    for (int i = 1; i <= 7; i += 3) {
        for (int j = 1; j <= 7; j += 3) {
            block_check.assign(BOARD_SIZE, 0);
            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    if (board_[i + x][j + y] == '.') {
                        continue;
                    }
                    if (block_check[get(board_[i + x][j + y])]++) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}