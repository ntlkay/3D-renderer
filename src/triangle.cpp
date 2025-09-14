#include "triangle.h"

namespace renderer {

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) : vertices_{v1, v2, v3} {
}

void Triangle::transform(const Matrix4& matrix) {
    for (auto& vertex : vertices_)
        vertex.transform(matrix);
}

const Vertex& Triangle::GetVertex(unsigned int index) const {
    return vertices_[index];
}

const std::array<Vertex, 3>& Triangle::getVertices() const {
    return vertices_;
}

std::array<Vector4, 3> Triangle::getPositions() const {
    return {vertices_[0].getPosition(), vertices_[1].getPosition(), vertices_[2].getPosition()};
}

std::array<Vector4, 3> Triangle::getNormals() const {
    return {vertices_[0].getNormal(), vertices_[1].getNormal(), vertices_[2].getNormal()};
}

iterator Triangle::begin() {
    return vertices_.begin();
}

iterator Triangle::end() {
    return vertices_.end();
}

iterator Triangle::begin() const {
    return vertices_.begin();
}

iterator Triangle::end() const {
    return vertices_.end();
}

}  // namespace renderer
