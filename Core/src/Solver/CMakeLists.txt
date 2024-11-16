cmake_minimum_required(VERSION 3.10)

add_library(Solver OBJECT
    Courant.cpp
    SolverBase.cpp
    LaplaceSolver.cpp
    ExplicitIterator.cpp
)

target_link_libraries(Solver VecPlus)

target_include_directories(Solver PUBLIC ${PUBLIC_INCLUDE_DIR})