cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6  REQUIRED COMPONENTS Widgets Gui OpenGL OpenGLWidgets)
qt_standard_project_setup()

qt6_wrap_cpp(MOC_FILES 
    ${PROJECT_SOURCE_DIR}/App/include/Simulation.h
    ${PROJECT_SOURCE_DIR}/App/include/SimulationGL.h
    )

qt_add_executable(helloworld
    src/main.cpp
    src/Simulation.cpp
    src/SimulationGL.cpp
    ${MOC_FILES}
)



target_link_libraries(helloworld PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    VecPlus
    Domain
 
    )

target_include_directories(helloworld PRIVATE
    ${PROJECT_SOURCE_DIR}/App/include
)

