#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

class Camera {
private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;

    glm::mat4 world_to_cam_;
    glm::mat4 cam_to_clip_;
    glm::mat4 world_to_clip_;

    double fov_;
    double aspect_;
    double near_;
    double far_;

    void updateViewMatrix();
    void updateCameraToClip();
    void updateWorldToClip();

public:
    Camera(double fov = 60.0, double aspect = 1.0, double near = 0.1, double far = 100.0);

    void setPosition(const glm::vec3& position);
    void rotate(float angle, const glm::vec3& axis);
    void move(const glm::vec3& offset);

    const glm::mat4& getWorldToClip() const;
};

}  // namespace renderer