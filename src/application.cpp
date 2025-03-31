#include "application.h"

namespace renderer {

Application::Application()
    : window_(sf::VideoMode({globals::WIDTH, globals::HEIGHT}), "3D Renderer"),
      screen_(globals::WIDTH, globals::HEIGHT),
      camera_(globals::FOV, globals::WIDTH / globals::HEIGHT, globals::NEAR, globals::FAR,
              globals::CAMERA_POSITION) {

    Object cube1(CubeCreator::create());
    world_.addObject(cube1);

    Object cube2(CubeCreator::create());
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 3.0f));
    cube2.move(translation);
    world_.addObject(cube2);
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
        float yawAngle = 0.0f;
        float pitchAngle = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            moveDir.z = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            moveDir.z = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            moveDir.x = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            moveDir.x = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            moveDir.y = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
            moveDir.y = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            yawAngle = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            yawAngle = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            pitchAngle = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            pitchAngle = -1.0f;

        if (moveDir.x != 0.0f || moveDir.y != 0.0f || moveDir.z != 0.0f) {
            moveDir = glm::normalize(moveDir);
        }

        if (moveDir != glm::vec3(0.0f)) {
            camera_.move(moveDir * globals::MOVEMENT_SPEED * deltaTime);
        }

        if (yawAngle != 0.0f) {
            camera_.rotate(yawAngle * globals::ROTATION_SPEED * deltaTime, glm::vec3(0, 1, 0));
        }
        if (pitchAngle != 0.0f) {
            camera_.rotate(pitchAngle * globals::ROTATION_SPEED * deltaTime, camera_.getCamX());
        }

        renderer_.render(world_, camera_, screen_);

        window_.clear();
        const auto& pixels = screen_.getPixels();
        window_.draw(pixels.data(), pixels.size(), sf::PrimitiveType::Points);
        window_.display();
    }
}

}  // namespace renderer
