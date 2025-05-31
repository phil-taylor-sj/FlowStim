cmake_minimum_required(VERSION 3.10)

include(FetchContent)
FetchContent_Declare(
  VecPlus
  GIT_REPOSITORY https://github.com/phil-taylor-sj/VecPlus.git
  GIT_TAG vec-add-cmake
)

FetchContent_MakeAvailable(VecPlus)

add_subdirectory(Domain)
add_subdirectory(Field)
add_subdirectory(Solver)
add_subdirectory(Case)