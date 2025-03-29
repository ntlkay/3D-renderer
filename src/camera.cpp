#include "camera.h"

namespace renderer {

Camera::Camera(double fov, double aspect, double near, double far)
    : fov_(fov),
      aspect_(aspect),
      near_(near),
      far_(far),
      position_(0.0f, 0.0f, 0.0f),
      front_(0.0f, 0.0f, -1.0f),
      up_(0.0f, 1.0f, 0.0f),
      right_(1.0f, 0.0f, 0.0f) {
    updateCameraToClip();
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    world_to_cam_ = glm::lookAt(position_, position_ + front_, up_);
    updateWorldToClip();
}

void Camera::updateCameraToClip() {
    cam_to_clip_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
    updateWorldToClip();
}

void Camera::updateWorldToClip() {
    world_to_clip_ = cam_to_clip_ * world_to_cam_;
}

void Camera::setPosition(const glm::vec3& position) {
    position_ = position;
    updateViewMatrix();
}

void Camera::rotate(float angle, const glm::vec3& axis) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    front_ = glm::normalize(glm::vec3(rotation * glm::vec4(front_, 0.0f)));
    right_ = glm::normalize(glm::vec3(rotation * glm::vec4(right_, 0.0f)));
    up_ = glm::normalize(glm::cross(right_, front_));

    updateViewMatrix();
}

void Camera::move(const glm::vec3& offset) {
    position_ += offset.x * right_ + offset.y * up_ - offset.z * front_;
    updateViewMatrix();
}

const glm::mat4& Camera::getWorldToClip() const {
    return world_to_clip_;
}

}  // namespace renderer