cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(APP_NAME FlowStimQML)

find_package(Qt6 COMPONENTS Quick Gui REQUIRED )
qt_standard_project_setup()

qt_add_resources(RESOURCE_FILES resources/qml.qrc)

qt_add_executable(${APP_NAME} ${RESOURCE_FILES}
    src/main.cpp
    ${MOC_FILES}
)

#qt_add_qml_module(${APP_NAME}
#    URI FlowStim
#    QML_FILES
#)

target_link_libraries(${APP_NAME} PRIVATE
    Qt6::Gui
    Qt6::Quick
    )

target_include_directories(${APP_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/AppQML/include
)

