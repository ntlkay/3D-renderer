cmake_minimum_required(VERSION 3.15)

project(3D-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

add_subdirectory(libs/glm)
add_subdirectory(libs/SFML)

include(src/sources.cmake)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_BINARY_DIR}"
)
