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
    FaceGradients.cpp
    PressureExplicit.cpp
    PisoSolver.cpp
    )



find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(Solver VecPlus OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found!")
    #target_link_libraries(${BENCH_TARGET} 
    #    benchmark::benchmark Domain Solver Field
    #)
endif()


target_include_directories(Solver PUBLIC ${PUBLIC_INCLUDE_DIR})