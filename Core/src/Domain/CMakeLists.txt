cmake_minimum_required(VERSION 3.10)

add_library(Domain OBJECT
    Mesh.cpp
    MeshFactory.cpp
    GridProfile.cpp
    GridProfileEquidistant.cpp
)

target_link_libraries(Domain VecPlus)

target_include_directories(Domain PUBLIC ${PUBLIC_INCLUDE_DIR})