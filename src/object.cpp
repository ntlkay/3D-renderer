#include "object.h"

namespace renderer {

Object::Object(std::vector<Triangle>&& triangles) : triangles_(std::move(triangles)) {
}

void Object::move(const Matrix4& matrix) {
    for (auto& triangle : triangles_) {
        triangle.transform(matrix);
    }
}

void Object::rotate(const Matrix4& matrix) {
    for (auto& triangle : triangles_) {
        triangle.transform(matrix);
    }
}

const std::vector<Triangle>& Object::getTriangles() const {
    return triangles_;
}

}  // namespace renderer
