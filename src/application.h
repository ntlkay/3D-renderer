#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "camera.h"
#include "globals.h"
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

public:
    Application();
    void run();
};

}  // namespace renderer
