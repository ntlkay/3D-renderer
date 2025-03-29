include_directories("${CMAKE_SOURCE_DIR}/libs/glm")
include_directories("${CMAKE_SOURCE_DIR}/libs/SFML/include")

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/parser.cpp
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