#include "application.h"

namespace renderer {

Application::Application()
    : window_(sf::VideoMode({WIDTH, HEIGHT}), "3D Renderer"),
      screen_(WIDTH, HEIGHT),
      camera_(FOV, WIDTH / HEIGHT, NEAR, FAR) {
    camera_.setPosition(glm::vec3(0, 0, 5));
    Object cube(CubeCreator::create());
    world_.addObject(cube);
}

void Application::run() {
    sf::Clock frameClock;

    while (window_.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();

        while (auto event = window_.pollEvent()) {
            if (event.has_value()) {
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                }
            }
        }

        glm::vec3 moveDir(0.0f);
        float rotateAngle = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            moveDir.z = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            moveDir.z = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            moveDir.x = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            moveDir.x = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            rotateAngle = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            rotateAngle = -1.0f;

        if (moveDir.x != 0.0f || moveDir.z != 0.0f) {
            moveDir = glm::normalize(moveDir);
        }

        if (moveDir != glm::vec3(0.0f)) {
            camera_.move(moveDir * MOVEMENT_SPEED * deltaTime);
        }

        if (rotateAngle != 0.0f) {
            camera_.rotate(rotateAngle * ROTATION_SPEED * deltaTime, glm::vec3(0, 1, 0));
        }

        renderer_.render(world_, camera_, screen_);

        window_.clear();
        const auto& pixels = screen_.getPixels();
        window_.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);
        window_.display();
    }
}

}  // namespace renderer
