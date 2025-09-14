#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "linear_algebra.h"
#include "triangle.h"

namespace renderer {

class Camera {
private:
    Matrix4 world_to_cam_;
    Matrix4 cam_to_clip_;
    Matrix4 world_to_clip_;

    float fov_;
    float aspect_;
    float near_;
    float far_;

    float movement_speed_;
    float rotation_speed_;

    void rotate(float angle, const glm::vec3& axis);

public:
    static constexpr float kFov = 60.0f;
    static constexpr float kAspect = 1.0f;
    static constexpr float kNear = 0.1f;
    static constexpr float kFar = 100.0f;
    static constexpr float kMovementSpeed = 1.0f;
    static constexpr float kRotationtSpeed = 30.0f;

    static constexpr glm::vec3 kCameraPosition = glm::vec3(0, 0, 5);

    Camera(float fov = kFov, float aspect = kAspect, float near = kNear, float far = kFar,
           glm::vec3 camera_position = glm::vec3(0, 0, 5), float movement_speed = kMovementSpeed,
           float rotation_speed = kRotationtSpeed);

    static Matrix4 makeWorldToCam(const glm::vec3& position, const glm::vec3& forward,
                                  const glm::vec3& up);

    static Matrix4 makeCameraToClip(float fov, float aspect, float near_plane, float far_plane);

    static Matrix4 makeWorldToClip(const Matrix4& world_to_cam, const Matrix4& cam_to_clip);

    void setAspect(float aspect);
    void setMovementSpeed(float movement_speed);
    void setRotationSpeed(float rotation_speed);

    void rotatePitch(float angle);
    void rotateYaw(float angle);
    void move(const glm::vec3& offset);

    glm::vec3 getCamX() const;
    glm::vec3 getCamY() const;
    glm::vec3 getCamZ() const;
    glm::vec3 getPosition() const;

    float getNear() const;
    float getFar() const;

    float getMovementSpeed() const;
    float getRotationSpeed() const;

    const Matrix4& getWorldToCam() const;
    const Matrix4& getCamToClip() const;
    const Matrix4& getWorldToClip() const;
};

}  // namespace renderer
