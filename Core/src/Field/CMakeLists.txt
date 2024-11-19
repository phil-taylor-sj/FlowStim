cmake_minimum_required(VERSION 3.10)

add_library(Field OBJECT
    Field.cpp
    FieldEquation.cpp
    Laplacian.cpp
    Divergence.cpp
    EulerExplicit.cpp
    FaceFlux.cpp
    FieldTolerance.cpp
)

target_link_libraries(Field VecPlus)

target_include_directories(Field PUBLIC ${PUBLIC_INCLUDE_DIR})