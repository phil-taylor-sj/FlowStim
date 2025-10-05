
set(PROFILE_TARGET FlowStim_Profile)

set(PROFILE_SRCS main_profile.cpp)

set(PROFILE_LIBS Domain Solver Field Case)

add_executable(${PROFILE_TARGET} ${PROFILE_SRCS})

target_include_directories(${PROFILE_TARGET} PUBLIC 
  ${PUBLIC_INCLUDE_DIR}
  ) 

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    list(APPEND ${PROFILE_LIBS}  OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found!")
endif()

target_link_libraries(${PROFILE_TARGET} ${PROFILE_LIBS})

set_target_properties(${PROFILE_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)
