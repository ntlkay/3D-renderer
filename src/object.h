#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "triangle.h"

namespace renderer {

class Object {
private:
    std::vector<Triangle> triangles_;

public:
    explicit Object(std::vector<Triangle> tris);

    void move(const glm::mat4& transformation);
    void rotate(const glm::mat4& rotation);

    const std::vector<Triangle>& getTriangles() const&;
};

}  // namespace renderer
