#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "globals.h"
#include "triangle.h"

namespace renderer {

class Camera {
private:
    glm::vec3 position_;
    glm::vec3 z_;
    glm::vec3 y_;
    glm::vec3 x_;

    glm::mat4 world_to_cam_;
    glm::mat4 cam_to_clip_;
    glm::mat4 world_to_clip_;

    double fov_;
    double aspect_;
    double near_;
    double far_;

    void updateWorldToCam();
    void updateCameraToClip();
    void updateWorldToClip();

public:
    Camera(double fov = 60.0, double aspect = 1.0, double near = 0.1, double far = 100.0,
           glm::vec3 camera_position = glm::vec3(0, 0, 5));

    void rotate(float angle, const glm::vec3& axis);
    void move(const glm::vec3& offset);
    std::vector<Triangle> clipTriangle(const Triangle& triangle) const;

    const glm::vec3& getCamX() const;
    const glm::vec3& getCamY() const;
    const glm::vec3& getCamZ() const;
    const glm::mat4& getWorldToClip() const;
};

}  // namespace renderer