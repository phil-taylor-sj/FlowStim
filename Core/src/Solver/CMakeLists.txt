cmake_minimum_required(VERSION 3.10)

add_library(Solver OBJECT
    Courant.cpp
    SolverBase.cpp
    BurgersSolver.cpp
    PointMethod.cpp
    ExplicitMethod.cpp
    JacobiMethod.cpp
    JacobiScalarMethod.cpp
    JacobiVectorMethod.cpp
    RhieChow.cpp
    FaceValues.cpp
    )

target_link_libraries(Solver VecPlus)

target_include_directories(Solver PUBLIC ${PUBLIC_INCLUDE_DIR})