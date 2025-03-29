#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <tuple>

#include "camera.h"
#include "parser.h"
#include "renderer.h"
#include "screen.h"
#include "vertex.h"
#include "world.h"

namespace renderer {

class Application {
private:
    sf::RenderWindow window_;
    World world_;
    Camera camera_;
    Screen screen_;
    Renderer renderer_;
    sf::Clock clock_;

    static constexpr unsigned int WIDTH = 800;
    static constexpr unsigned int HEIGHT = 600;
    static constexpr float FOV = 60.0f;
    static constexpr float NEAR = 0.1f;
    static constexpr float FAR = 100.0f;
    static constexpr float ROTATION_SPEED = 30.0f;
    static constexpr float MOVEMENT_SPEED = 2.0f;

public:
    Application();
    void run();
};

}  // namespace renderer
