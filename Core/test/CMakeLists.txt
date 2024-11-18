cmake_minimum_required(VERSION 3.10)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_TARGET FlowStim_Tests)

add_subdirectory(Domain_Test)

add_subdirectory(Solver_Test)

set(TEST_SRCS
  pch.cpp
  pch.h
  Domain_Test/MeshFactory_Test.cpp
  Solver_Test/JacobiScalarMethod_Test.cpp
  )


add_executable(${TEST_TARGET} ${TEST_SRCS})
target_link_libraries(${TEST_TARGET} 
  gtest gtest_main gmock_main Domain Solver Field
  )
target_include_directories(${TEST_TARGET} PUBLIC 
  ${PUBLIC_INCLUDE_DIR}
  ) 

set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})