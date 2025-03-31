#include "camera.h"

namespace renderer {

Camera::Camera(double fov, double aspect, double near, double far, glm::vec3 camera_position)
    : fov_(fov),
      aspect_(aspect),
      near_(near),
      far_(far),
      position_(camera_position),
      z_(0.0f, 0.0f, -1.0f),
      y_(0.0f, 1.0f, 0.0f),
      x_(1.0f, 0.0f, 0.0f) {
    updateCameraToClip();
    updateWorldToCam();
}

void Camera::updateWorldToCam() {
    world_to_cam_ = glm::lookAt(position_, position_ + z_, y_);
    updateWorldToClip();
}

void Camera::updateCameraToClip() {
    cam_to_clip_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
    updateWorldToClip();
}

void Camera::updateWorldToClip() {
    world_to_clip_ = cam_to_clip_ * world_to_cam_;
}

void Camera::rotate(float angle, const glm::vec3& axis) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    z_ = glm::normalize(glm::vec3(rotation * glm::vec4(z_, 0.0f)));

    if (axis == glm::vec3(1.0f, 0.0f, 0.0f)) {
        float current_pitch = glm::degrees(asin(z_.y));
        float new_pitch = current_pitch + angle;

        if (new_pitch > 89.0f) {
            angle = 89.0f - current_pitch;
            rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
            z_ = glm::normalize(glm::vec3(rotation * glm::vec4(z_, 0.0f)));
        } else if (new_pitch < -89.0f) {
            angle = -89.0f - current_pitch;
            rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
            z_ = glm::normalize(glm::vec3(rotation * glm::vec4(z_, 0.0f)));
        }
    }

    x_ = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z_));
    y_ = glm::normalize(glm::cross(z_, x_));

    updateWorldToCam();
}

void Camera::move(const glm::vec3& offset) {
    glm::vec3 forward = glm::normalize(glm::vec3(z_.x, 0.0f, z_.z));
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));

    position_ += offset.z * forward + offset.x * right + offset.y * glm::vec3(0.0f, 1.0f, 0.0f);

    updateWorldToCam();
}

std::vector<Triangle> Camera::clipTriangle(const Triangle& triangle) const {
    std::array<bool, 3> is_visible;
    std::array<Vector4, 3> positions;
    for (int i = 0; i < 3; ++i) {
        positions[i] = triangle[i].getPosition();
        is_visible[i] = (positions[i].z >= near_);
    }

    if (is_visible[0] && is_visible[1] && is_visible[2]) {
        return {triangle};
    }

    if (!is_visible[0] && !is_visible[1] && !is_visible[2]) {
        return {};
    }

    int visible_count = is_visible[0] + is_visible[1] + is_visible[2];
    int back_idx1, back_idx2, front_idx;

    if (visible_count == 1) {
        for (int i = 0; i < 3; ++i) {
            if (is_visible[i]) {
                front_idx = i;
                back_idx1 = (i + 1) % 3;
                back_idx2 = (i + 2) % 3;
                break;
            }
        }

        Vertex front = triangle[front_idx];
        Vertex back1 = triangle[back_idx1];
        Vertex back2 = triangle[back_idx2];

        double t1 =
            (near_ - back1.getPosition().z) / (front.getPosition().z - back1.getPosition().z);
        double t2 =
            (near_ - back2.getPosition().z) / (front.getPosition().z - back2.getPosition().z);

        Vertex intersect1 = back1.interpolate(front, t1);
        Vertex intersect2 = back2.interpolate(front, t2);

        return {Triangle{front, intersect1, intersect2}};
    }
    for (int i = 0; i < 3; ++i) {
        if (!is_visible[i]) {
            back_idx1 = i;
            front_idx = (i + 1) % 3;
            front_idx = is_visible[front_idx] ? front_idx : (i + 2) % 3;
            back_idx2 = 3 - back_idx1 - front_idx;
            break;
        }
    }

    Vertex front1 = triangle[front_idx];
    Vertex front2 = triangle[back_idx2];
    Vertex back = triangle[back_idx1];

    double t1 = (near_ - back.getPosition().z) / (front1.getPosition().z - back.getPosition().z);
    double t2 = (near_ - back.getPosition().z) / (front2.getPosition().z - back.getPosition().z);

    Vertex intersect1 = back.interpolate(front1, t1);
    Vertex intersect2 = back.interpolate(front2, t2);

    return {
        Triangle{front1, front2, intersect1},
        Triangle{front2, intersect1, intersect2},
    };
}

const glm::vec3& Camera::getCamX() const {
    return x_;
}

const glm::vec3& Camera::getCamY() const {
    return y_;
}

const glm::vec3& Camera::getCamZ() const {
    return z_;
}

const glm::mat4& Camera::getWorldToClip() const {
    return world_to_clip_;
}

}  // namespace renderer