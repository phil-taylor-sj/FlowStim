cmake_minimum_required(VERSION 3.10)

add_library(Domain OBJECT
    Mesh.cpp
    Face.cpp
    Cell.cpp
    Vertex.cpp
    MeshFactory.cpp
    Mesh2dStructuredFactory.cpp
    GridProfile.cpp
    GridProfileEquidistant.cpp
    FaceSetFactory.cpp
    VertexMapping.cpp
    CompassUtils.cpp
)

target_link_libraries(Domain VecPlus)

target_include_directories(Domain PUBLIC ${PUBLIC_INCLUDE_DIR})