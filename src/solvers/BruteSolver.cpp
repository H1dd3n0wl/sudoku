#include "BruteSolver.hpp"

void BruteSolver::resetChange(int row, int col) {
    for (auto [cord, erased] : updated_cells[getIndex(row, col)]) {
        auto [x, y] = getCoordinates(cord);
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
            } else {
                possible_nums[i][j] = {intFromChar(board[i][j])};
            }
        }
    }
}

BruteSolver::BruteSolver(const std::string& file)
    : board_(BOARD_SIZE, std::vector<char>(BOARD_SIZE)),
      possible_nums(BOARD_SIZE,
                    std::vector<std::unordered_set<int>>(BOARD_SIZE, {1, 2, 3, 4, 5, 6, 7, 8, 9})),
      empty_cells{} {
    std::ifstream in(file);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            in >> board_[i][j];
            if (!isdigit(board_[i][j])) {
                empty_cells.insert(getIndex(i, j));
            } else {
                possible_nums[i][j] = {intFromChar(board_[i][j])};
            }
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

void BruteSolver::tryUpdate(int row, int col) {
    cells_stack.emplace_back(getIndex(row, col), possible_nums[row][col]);
    while (!cells_stack.empty()) {
        auto [x, y] = getCoordinates(cells_stack.back().first);
        if (update(x, y)) {
            break;
        } else {
            possible_nums[x][y] = cells_stack.back().second;
            cells_stack.pop_back();
            if (!cells_stack.empty()) {
                auto [prev_x, prev_y] = getCoordinates(cells_stack.back().first);
                possible_nums[prev_x][prev_y] = cells_stack.back().second;
                possible_nums[prev_x][prev_y].erase(intFromChar(board_[prev_x][prev_y]));
                board_[prev_x][prev_y] = '.';
            }
        }
    }
}

bool BruteSolver::update(int row, int col) {
    empty_cells.erase(getIndex(row, col));
    bool goodUpdate = false;
    for (auto num : possible_nums[row][col]) {
        board_[row][col] = charFromInt(num);
        if (hardUpdate(row, col)) {
            goodUpdate = true;
            break;
        } else {
            resetChange(row, col);
        }
    }
    if (goodUpdate) {
        possible_nums[row][col] = {intFromChar(board_[row][col])};
    } else {
        board_[row][col] = '.';
        empty_cells.insert(getIndex(row, col));
    }
    return goodUpdate;
}

bool BruteSolver::hardUpdate(int row, int col) {
    int to_erase = intFromChar(board_[row][col]);
    int cord = getIndex(row, col);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (i != col && possible_nums[row][i].find(to_erase) != std::end(possible_nums[row][i])) {
            possible_nums[row][i].erase(to_erase);
            updated_cells[cord].emplace_back(getIndex(row, i), to_erase);
            if (possible_nums[row][i].empty()) {
                return false;
            }
        }
        if (i != row && possible_nums[i][col].find(to_erase) != std::end(possible_nums[i][col])) {
            possible_nums[i][col].erase(to_erase);
            updated_cells[cord].emplace_back(getIndex(i, col), to_erase);
            if (possible_nums[i][col].empty()) {
                return false;
            }
        }
    }

    int block_x = (row / 3) * 3 + 1;
    int block_y = (col / 3) * 3 + 1;

    for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
            if (block_x + x == row && block_y + y == col) {
                continue;
            }
            if (possible_nums[block_x + x][block_y + y].find(to_erase) !=
                std::end(possible_nums[block_x + x][block_y + y])) {
                possible_nums[block_x + x][block_y + y].erase(to_erase);
                updated_cells[cord].emplace_back(getIndex(block_x + x, block_y + y), to_erase);
                if (possible_nums[block_x + x][block_y + y].empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

void BruteSolver::solve() {
    while (needToSolve()) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (!isdigit(board_[i][j])) {
                    tryUpdate(i, j);
                }
            }
        }
        printBoard();
        std::getchar();
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

std::vector<std::vector<char>> BruteSolver::getBoard() { return board_; }
