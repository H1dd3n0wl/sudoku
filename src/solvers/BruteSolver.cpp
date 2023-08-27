#include "BruteSolver.hpp"

BruteSolver::BruteSolver(std::vector<std::vector<char>> board)
    : board_{board},
      possible_nums(BOARD_SIZE,
                    std::vector<std::unordered_set<int>>(BOARD_SIZE, {1, 2, 3, 4, 5, 6, 7, 8, 9})),
      updated_cells(BOARD_SIZE, std::vector<std::unordered_set<int>>(BOARD_SIZE)),
      used_dfs(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false)),
      empty_cells{},
      cells_stack{} {
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
    bool good;
    for (auto num : possible_nums[row][col]) {
        good = true;
        board_[row][col] = charFromInt(num);
        bool continue_iteration = true;
        while (continue_iteration) {
            auto [r, c] = getCoordinates(cells_stack.back());
            for (int i = 0; i < BOARD_SIZE; ++i) {
                if (!isdigit(board_[r][i])) {
                    if (possible_nums[r][i].find(num) != std::end(possible_nums[r][i])) {
                        possible_nums[r][i].erase(num);
                        updated_cells[r][i].insert(num);
                        if (!used_dfs[r][i]) {
                            used_dfs[r][i] = true;
                            cells_stack.push_back(getIndex(r, i));
                        }
                    }
                }
                if (!isdigit(board_[i][c])) {
                    if (possible_nums[i][c].find(num) != std::end(possible_nums[r][i])) {
                        possible_nums[i][c].erase(num);
                        updated_cells[i][c].insert(num);
                        if (!used_dfs[i][c]) {
                            used_dfs[i][c] = true;
                            cells_stack.push_back(getIndex(i, c));
                        }
                    }
                }
            }

            int block_x = (r / 3) * 3 + 1;
            int block_y = (c / 3) * 3 + 1;

            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    if (isdigit(board_[block_x + x][block_y + y])) {
                        continue;
                    }
                    if (possible_nums[block_x + x][block_y + y].find(num) !=
                        std::end(possible_nums[block_x + x][block_y + y])) {
                        possible_nums[block_x + x][block_y + y].erase(num);
                        updated_cells[block_x + x][block_y + y].insert(num);
                        if (!used_dfs[block_x + x][block_y + y]) {
                            used_dfs[block_x + x][block_y + y] = true;
                            cells_stack.push_back(getIndex(block_x + x, block_y + y));
                        }
                    }
                }
            }
        }
    }
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

    int previous_cell = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!isdigit(board_[i][j])) {
                cells_stack.push_back(getIndex(i, j));
                hardUpdate(i, j, previous_cell);
                if (previous_cell == -1) {
                    previous_cell = getIndex(i, j);
                }
            }
        }
    }
}
