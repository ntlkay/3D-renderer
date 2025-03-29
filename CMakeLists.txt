cmake_minimum_required(VERSION 3.15)

project(3D-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glm)
add_subdirectory(libs/SFML)

include(src/sources.cmake)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)
