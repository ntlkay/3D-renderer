include_directories("${CMAKE_SOURCE_DIR}/libs/glm")
include_directories("${CMAKE_SOURCE_DIR}/libs/SFML/include")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/except.cpp
    src/parser.cpp
    src/vector3.cpp
	src/vector4.cpp
    src/vertex.cpp
    src/triangle.cpp
    src/object.cpp
    src/camera.cpp
    src/world.cpp
    src/screen.cpp
    src/renderer.cpp
	src/application.cpp
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/resources
)
