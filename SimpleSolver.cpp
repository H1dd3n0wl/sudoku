#include "SimpleSolver.hpp"

SimpleSolver::SimpleSolver(std::vector<std::vector<char>> board)
    : board_{board},
      possible_numbers(BOARD_SIZE, std::vector<std::set<int>>(
                                       BOARD_SIZE, {1, 2, 3, 4, 5, 6, 7, 8, 9})) {}

void SimpleSolver::prepare() {
    auto get([](char c) {
        return (int)(c - '0');
    });

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!isdigit(board_[i][j])) {
                continue;
            }
            int to_remove = get(board_[i][j]);
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (!isdigit(board_[x][j])) {
                    possible_numbers[x][j].erase(to_remove);
                }
                if (!isdigit(board_[i][x])) {
                    possible_numbers[i][x].erase(to_remove);
                }
            }

            int block_x = (i / 3) * 3 + 1;
            int block_y = (j / 3) * 3 + 1;

            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    if (!isdigit(board_[block_x + x][block_y + y])) {
                        possible_numbers[block_x + x][block_y + y].erase(to_remove);
                    }
                }
            }
        }
    }

     auto set([](int x) {
        return (char)(x + '0');
    });

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (possible_numbers[i][j].size() == 1u) {
                board_[i][j] = set(*(std::begin(possible_numbers[i][j])));
            }
        }
    }

}

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