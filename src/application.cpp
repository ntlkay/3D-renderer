#include "application.h"

#include "linear_algebra.h"

namespace renderer {

World Application::populate_world() {
    World world;

    Object cube1 = CubeCreator::create();
    world.addObject(std::move(cube1));

    Object cube2 =
        CubeCreator::create(1.0, glm::translate(Matrix4(1.0f), glm::vec3(2.0f, 0.0f, 3.0f)));
    world.addObject(std::move(cube2));

    Object obj = ObjParser::parse("../resources/teapot.obj", sf::Color::Green);
    world.addObject(std::move(obj));

    return world;
}

Application::Application()
    : window_(sf::VideoMode({kWidth, kHeight}), kWindowName),
      world_(populate_world()),
      screen_(kWidth, kHeight),
      camera_(Camera::kFov, static_cast<float>(kWidth) / static_cast<float>(kHeight), Camera::kNear,
              Camera::kFar, Camera::kCameraPosition) {
}

InputData Application::read_input() {
    InputData data{};
    data.moveDir = glm::vec3(0.0f);
    data.yawAngle = 0.0f;
    data.pitchAngle = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        data.moveDir.z = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        data.moveDir.z = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        data.moveDir.x = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        data.moveDir.x = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        data.moveDir.y = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        data.moveDir.y = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        data.yawAngle = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        data.yawAngle = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        data.pitchAngle = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        data.pitchAngle = -1.0f;

    return data;
}

void Application::run() {
    sf::Clock frameClock;

    while (window_.isOpen()) {
        while (auto event = window_.pollEvent()) {
            if (event.has_value()) {
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                } else if (event->is<sf::Event::Resized>()) {
                    auto resizeEvent = event->getIf<sf::Event::Resized>();
                    if (resizeEvent) {
                        int newWidth = resizeEvent->size.x;
                        int newHeight = resizeEvent->size.y;

                        window_.setView(sf::View(sf::FloatRect(
                            sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(newWidth),
                                                                 static_cast<float>(newHeight)))));
                        screen_ = Screen(Width{newWidth}, Height{newHeight});
                        camera_.setAspect(newWidth / newHeight);
                    }
                }
            }
        }
        float deltaTime = frameClock.restart().asSeconds();

        auto move_data = read_input();

        camera_.move(move_data.moveDir * deltaTime);

        if (std::fabs(move_data.yawAngle) > std::numeric_limits<float>::epsilon()) {
            camera_.rotateYaw(move_data.yawAngle * deltaTime);
        }
        if (std::fabs(move_data.pitchAngle) > std::numeric_limits<float>::epsilon()) {
            camera_.rotatePitch(move_data.pitchAngle * deltaTime);
        }

        screen_ = renderer_.render(world_, camera_, std::move(screen_));

        window_.clear();
        screen_.draw(window_);
        window_.display();
    }
}

}  // namespace renderer
