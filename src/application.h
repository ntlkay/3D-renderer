#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "camera.h"
#include "parser.h"
#include "renderer.h"
#include "screen.h"
#include "vertex.h"
#include "world.h"

namespace renderer {

struct InputData {
    glm::vec3 moveDir;
    float yawAngle;
    float pitchAngle;
};

class Application {
private:
    static constexpr Width kWidth{800};
    static constexpr Height kHeight{600};
    static constexpr char kWindowName[] = "3D Renderer";

    sf::RenderWindow window_;
    World world_;
    Camera camera_;
    Screen screen_;
    Renderer renderer_;
    sf::Clock clock_;

    static World populate_world();
    static InputData read_input();

public:
    Application();
    void run();
};

}  // namespace renderer
