#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "linear_algebra.h"
#include "triangle.h"

namespace renderer {

class Object {
private:
    std::vector<Triangle> triangles_;

public:
    explicit Object(std::vector<Triangle>&& triangles);

    void move(const Matrix4& transformation);
    void rotate(const Matrix4& rotation);

    const std::vector<Triangle>& getTriangles() const;
};

}  // namespace renderer
