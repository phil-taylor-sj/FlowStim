cmake_minimum_required(VERSION 3.10)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.7.1
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

set(BENCH_TARGET FlowStim_Benchmarks)

set(BENCH_SRCS
    main_benchmark.cpp
    Domain_Benchmark/Mesh2dStructuredFactory_Benchmark.cpp
    Solver_Benchmark/JacobiScalarMethod_Benchmark.cpp
    Solver_Benchmark/JacobiVectorMethod_Benchmark.cpp
    Solver_Benchmark/Case2dCavityFlow_Benchmark.cpp
)

add_executable(${BENCH_TARGET} ${BENCH_SRCS})

set(BENCH_LIBS
    benchmark::benchmark
    Domain
    Solver
    Field 
    Case
)

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    list(APPEND ${BENCH_LIBS} OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found!")
endif()

target_link_libraries(${BENCH_TARGET} ${BENCH_LIBS})

target_include_directories(${BENCH_TARGET} PUBLIC 
  ${PUBLIC_INCLUDE_DIR}
  ) 

set_target_properties(${BENCH_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)



