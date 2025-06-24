cmake_minimum_required(VERSION 3.10)

project(FlowStim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PROJECT_IS_TOP_LEVEL ON)
else()
    set(PROJECT_IS_TOP_LEVEL OFF)
endif()

set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)

set(BUILD_BIN_DIR ${PROJECT_SOURCE_DIR}/build/bin)

set(PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Core/include)

add_subdirectory(Core/src)
add_subdirectory(App)
#add_subdirectory(AppQML)

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(Core/test)
endif()