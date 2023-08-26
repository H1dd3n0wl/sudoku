include_directories(solvers)

set(SOURCES SimpleSolver.cpp)
set(HEADERS SimpleSolver.hpp)

add_library(SimpleSolver STATIC ${SOURCES} ${HEADERS})
