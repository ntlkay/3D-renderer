#include "camera.h"

namespace renderer {

Camera::Camera(float fov, float aspect, float near, float far, glm::vec3 camera_position,
               float movement_speed, float rotation_speed)
    : fov_(fov),
      aspect_(aspect),
      near_(near),
      far_(far),
      movement_speed_(movement_speed),
      rotation_speed_(rotation_speed) {
    world_to_cam_ = makeWorldToCam(camera_position, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    cam_to_clip_ = makeCameraToClip(fov_, aspect_, near_, far_);
    world_to_clip_ = makeWorldToClip(world_to_cam_, cam_to_clip_);
}

Matrix4 Camera::makeWorldToCam(const glm::vec3& position, const glm::vec3& forward,
                               const glm::vec3& up) {
    return glm::lookAt(position, position + forward, up);
}

Matrix4 Camera::makeCameraToClip(float fov, float aspect, float near, float far) {
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

Matrix4 Camera::makeWorldToClip(const Matrix4& world_to_cam, const Matrix4& cam_to_clip) {
    return cam_to_clip * world_to_cam;
}

glm::vec3 Camera::getPosition() const {
    Matrix4 cam_to_world = glm::inverse(world_to_cam_);
    return glm::vec3(cam_to_world[3]);
}

glm::vec3 Camera::getCamX() const {
    Matrix4 cam_to_world = glm::inverse(world_to_cam_);
    return glm::normalize(glm::vec3(cam_to_world[0]));
}

glm::vec3 Camera::getCamY() const {
    Matrix4 cam_to_world = glm::inverse(world_to_cam_);
    return glm::normalize(glm::vec3(cam_to_world[1]));
}

glm::vec3 Camera::getCamZ() const {
    Matrix4 cam_to_world = glm::inverse(world_to_cam_);
    return -glm::normalize(glm::vec3(cam_to_world[2]));
}

void Camera::rotate(float angle, const glm::vec3& axis) {
    glm::vec3 pos = getPosition();
    glm::vec3 forward = getCamZ();
    glm::vec3 up = getCamY();

    Matrix4 rotation = glm::rotate(Matrix4(1.0f), glm::radians(angle), axis);
    forward = glm::normalize(glm::vec3(rotation * glm::vec4(forward, 0.0f)));
    up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));

    world_to_cam_ = makeWorldToCam(pos, forward, up);
    world_to_clip_ = makeWorldToClip(world_to_cam_, cam_to_clip_);
}

void Camera::rotatePitch(float angle) {
    rotate(angle * rotation_speed_, getCamX());
}

void Camera::rotateYaw(float angle) {
    rotate(angle * rotation_speed_, getCamY());
}

void Camera::move(const glm::vec3& offset) {
    glm::vec3 pos = getPosition();
    glm::vec3 right = getCamX();
    glm::vec3 up = getCamY();
    glm::vec3 forward = getCamZ();

    pos += (offset.x * right + offset.y * up + offset.z * forward) * movement_speed_;

    world_to_cam_ = makeWorldToCam(pos, forward, up);
    world_to_clip_ = makeWorldToClip(world_to_cam_, cam_to_clip_);
}

void Camera::setAspect(float aspect) {
    aspect_ = aspect;
    cam_to_clip_ = makeCameraToClip(fov_, aspect_, near_, far_);
    world_to_clip_ = makeWorldToClip(world_to_cam_, cam_to_clip_);
}

void Camera::setMovementSpeed(float movement_speed) {
    movement_speed_ = movement_speed;
}

void Camera::setRotationSpeed(float rotation_speed) {
    rotation_speed_ = rotation_speed;
}

float Camera::getNear() const {
    return near_;
}
float Camera::getFar() const {
    return far_;
}
float Camera::getMovementSpeed() const {
    return movement_speed_;
}
float Camera::getRotationSpeed() const {
    return rotation_speed_;
}
const Matrix4& Camera::getWorldToCam() const {
    return world_to_cam_;
}
const Matrix4& Camera::getCamToClip() const {
    return cam_to_clip_;
}
const Matrix4& Camera::getWorldToClip() const {
    return world_to_clip_;
}

}  // namespace renderer
