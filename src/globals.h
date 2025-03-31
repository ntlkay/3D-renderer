#pragma once

namespace renderer {

namespace globals {

static constexpr unsigned int WIDTH = 800;
static constexpr unsigned int HEIGHT = 600;
static constexpr float FOV = 60.0f;
static constexpr float NEAR = 0.1f;
static constexpr float FAR = 100.0f;
static constexpr float ROTATION_SPEED = 30.0f;
static constexpr float MOVEMENT_SPEED = 1.0f;
const glm::vec3 CAMERA_POSITION = glm::vec3(0, 0, 5);

}  // namespace globals

}  // namespace renderer