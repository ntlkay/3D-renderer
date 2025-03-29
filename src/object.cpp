#include "object.h"

namespace renderer {

Object::Object(std::vector<Triangle> tris) : triangles_(std::move(tris)) {
}

void Object::move(const glm::mat4& matrix) {
    for (auto& tri : triangles_) {
        tri = tri.transform(matrix);
    }
}

void Object::rotate(const glm::mat4& matrix) {
    for (auto& tri : triangles_) {
        tri = tri.transform(matrix);
    }
}

const std::vector<Triangle>& Object::getTriangles() const& {
    return triangles_;
}

}  // namespace renderer
