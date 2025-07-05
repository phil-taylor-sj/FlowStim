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



find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(${BENCH_TARGET} 
        benchmark::benchmark Domain Solver Field Case OpenMP::OpenMP_CXX
    )
else()
    message(FATAL_ERROR "OpenMP not found!")
endif()

target_include_directories(${BENCH_TARGET} PUBLIC 
  ${PUBLIC_INCLUDE_DIR}
  ) 

set_target_properties(${BENCH_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)


#set(PROFILE_TARGET FlowStim_Profile)

#set(PROFILE_SRCS main_profile.cpp)

#add_executable(${PROFILE_TARGET} ${PROFILE_SRCS})

#target_include_directories(${PROFILE_TARGET} PUBLIC 
#  ${PUBLIC_INCLUDE_DIR}
#  ) 

#if (OpenMP_CXX_FOUND)
#    target_link_libraries(${PROFILE_TARGET} 
#        Domain Solver Field Case OpenMP::OpenMP_CXX
#    )
#else()
#    message(FATAL_ERROR "OpenMP not found!")
#endif()


#set_target_properties(${PROFILE_TARGET} PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
#)


