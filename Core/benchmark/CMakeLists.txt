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
)

add_executable(${BENCH_TARGET} ${BENCH_SRCS})

target_link_libraries(${BENCH_TARGET} 
    benchmark::benchmark Domain Solver Field
  )
target_include_directories(${BENCH_TARGET} PUBLIC 
  ${PUBLIC_INCLUDE_DIR}
  ) 

set_target_properties(${BENCH_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)