#include "BruteSolver.hpp"

BruteSolver::BruteSolver(std::vector<std::vector<char>> board)
    : board_{board},
      possible_nums(BOARD_SIZE,
                    std::vector<std::unordered_set<int>>(BOARD_SIZE, {1, 2, 3, 4, 5, 6, 7, 8, 9})),
      empty_cells{} {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!isdigit(board[i][j])) {
                empty_cells.insert(getIndex(i, j));
            }
        }
    }
}

void BruteSolver::usualUpdate(int row, int col) {
    empty_cells.erase(getIndex(row, col));
    int to_erase = intFromChar(board_[row][col]);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (!isdigit(board_[row][i])) {
            possible_nums[row][i].erase(to_erase);
            if (possible_nums[row][i].size() == 1u) {
                board_[row][i] = charFromInt(*(std::begin(possible_nums[row][i])));
                usualUpdate(row, i);
            }
        }
        if (!isdigit(board_[i][col])) {
            possible_nums[i][col].erase(to_erase);
            if (possible_nums[i][col].size() == 1u) {
                board_[i][col] = charFromInt(*(std::begin(possible_nums[i][col])));
                usualUpdate(i, col);
            }
        }
    }

    int block_x = (row / 3) * 3 + 1;
    int block_y = (col / 3) * 3 + 1;

    for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
            if (isdigit(board_[block_x + x][block_y + y])) {
                continue;
            }
            possible_nums[block_x + x][block_y + y].erase(to_erase);
            if (possible_nums[block_x + x][block_y + y].size() == 1u) {
                board_[block_x + x][block_y + y] =
                    charFromInt(*(std::begin(possible_nums[block_x + x][block_y + y])));
                usualUpdate(block_x + x, block_y + y);
            }
        }
    }
}

// !!!!!not working yet!!!!!
void BruteSolver::hardUpdate(int row, int col, int previous_cell) {

}

void BruteSolver::solve() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (isdigit(board_[i][j])) {
                usualUpdate(i, j);
            }
            if (!needToSolve()) {
                return;
            }
        }
    }
}
