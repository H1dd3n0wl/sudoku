#include "BruteSolver.hpp"

void BruteSolver::resetChange(int row, int col) {
    for (auto [cord, erased] : updated_cells[getIndex(row, col)]) {
        auto [x, y] = getCoordinates(cord);
        if (isdigit(board_[x][y])) {
            board_[x][y] = '.';
            empty_cells.insert(getIndex(x, y));
        }
        possible_nums[x][y].insert(erased);
    }
    updated_cells.erase(getIndex(row, col));
}

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

BruteSolver::BruteSolver(const std::string& file) : board_(BOARD_SIZE, std::vector<char>(BOARD_SIZE)) {
    std::ifstream in(file);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            in >> board_[i][j];
        }
    }
}

void BruteSolver::simpleUpdate(int row, int col) {
    empty_cells.erase(getIndex(row, col));
    int to_erase = intFromChar(board_[row][col]);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (!isdigit(board_[row][i])) {
            possible_nums[row][i].erase(to_erase);
            if (possible_nums[row][i].size() == 1u) {
                board_[row][i] = charFromInt(*(std::begin(possible_nums[row][i])));
                simpleUpdate(row, i);
            }
        }
        if (!isdigit(board_[i][col])) {
            possible_nums[i][col].erase(to_erase);
            if (possible_nums[i][col].size() == 1u) {
                board_[i][col] = charFromInt(*(std::begin(possible_nums[i][col])));
                simpleUpdate(i, col);
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
                simpleUpdate(block_x + x, block_y + y);
            }
        }
    }
}

bool BruteSolver::update(int row, int col) {
    for (auto num : possible_nums[row][col]) {
        board_[row][col] = charFromInt(num);
        if (hardUpdate(row, col)) {
            return true;
        } else {
            resetChange(row, col);
        }
    }
    return false;
}

bool BruteSolver::hardUpdate(int row, int col) {
    int cord = getIndex(row, col);
    std::vector<int> local_stack;
    local_stack.push_back(cord);
    while (!local_stack.empty()) {
        auto [r, c] = getCoordinates(local_stack.back());
        int to_erase = intFromChar(board_[r][c]);
        local_stack.pop_back();
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (!isdigit(board_[r][i])) {
                if (possible_nums[r][i].find(to_erase) != std::end(possible_nums[r][i])) {
                    possible_nums[r][i].erase(to_erase);
                    updated_cells[cord].emplace_back(getIndex(r, i), to_erase);
                    if (possible_nums[r][i].size() == 1u) {
                        board_[r][i] = charFromInt(*(std::begin(possible_nums[r][i])));
                        empty_cells.erase(getIndex(r, i));
                        local_stack.push_back(getIndex(r, i));
                    } else if (possible_nums[i][c].empty()) {
                        return false;
                    }
                }
            }

            if (!isdigit(board_[i][c])) {
                if (possible_nums[i][c].find(to_erase) != std::end(possible_nums[i][c])) {
                    possible_nums[i][c].erase(to_erase);
                    updated_cells[cord].emplace_back(getIndex(i, c), to_erase);
                    if (possible_nums[i][c].size() == 1u) {
                        board_[i][c] = charFromInt(*(std::begin(possible_nums[i][c])));
                        empty_cells.erase(getIndex(i, c));
                        local_stack.push_back(getIndex(i, c));
                    } else if (possible_nums[i][c].empty()) {
                        return false;
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
                if (possible_nums[block_x + x][block_y + y].find(to_erase) ==
                    std::end(possible_nums[block_x + x][block_y + y])) {
                    continue;
                }
                possible_nums[block_x + x][block_y + y].erase(to_erase);
                updated_cells[cord].emplace_back(getIndex(block_x + x, block_y + y), to_erase);
                if (possible_nums[block_x + x][block_y + y].size() == 1u) {
                    board_[block_x + x][block_y + y] =
                        charFromInt(*(std::begin(possible_nums[block_x + x][block_y + y])));
                    empty_cells.erase(getIndex(block_x + x, block_y + y));
                    local_stack.push_back(getIndex(block_x + x, block_y + y));
                } else if (possible_nums[block_x + x][block_y + y].empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

void BruteSolver::solve() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (isdigit(board_[i][j])) {
                simpleUpdate(i, j);
            }
            if (!needToSolve()) {
                return;
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!isdigit(board_[i][j])) {
                update(i, j);
                return;
            }
        }
    }
}

void BruteSolver::printBoard() {
    for (const auto& i : board_) {
        for (const auto& j : i) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }
}
