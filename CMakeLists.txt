cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(sudoku)

add_executable(sudoku src/main.cpp
                        src/solvers/SimpleSolver.cpp
                        src/solvers/BruteSolver.cpp)
