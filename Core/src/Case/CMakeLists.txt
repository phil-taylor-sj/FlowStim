cmake_minimum_required(VERSION 3.10)

add_library(Case OBJECT
    CaseFactory.cpp
    Case2dStructuredFactory.cpp
    Case2dCouetteFlowFactory.cpp
    #Case2dCavityFlowFactory.cpp
)

target_link_libraries(Case VecPlus)

target_include_directories(Case PUBLIC ${PUBLIC_INCLUDE_DIR})