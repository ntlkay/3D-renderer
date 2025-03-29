#include "triangle.h"

namespace renderer {

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) : vertices_{v1, v2, v3} {
}

Triangle Triangle::transform(const glm::mat4& matrix) const {
    return Triangle{
        Vertex{Vector4{matrix * glm::vec4(vertices_[0].getPosition())}, vertices_[0].getColor()},
        Vertex{Vector4{matrix * glm::vec4(vertices_[1].getPosition())}, vertices_[1].getColor()},
        Vertex{Vector4{matrix * glm::vec4(vertices_[2].getPosition())}, vertices_[2].getColor()}};
}

const Vertex& Triangle::operator[](size_t index) const {
    return vertices_[index];
}

const std::array<Vertex, 3>& Triangle::getVertices() const {
    return vertices_;
}
std::array<Vector4, 3> Triangle::getPositions() const {
    return {vertices_[0].getPosition(), vertices_[1].getPosition(), vertices_[2].getPosition()};
}

std::array<Vertex, 3>::iterator Triangle::begin() {
    return vertices_.begin();
}
std::array<Vertex, 3>::iterator Triangle::end() {
    return vertices_.end();
}
std::array<Vertex, 3>::const_iterator Triangle::begin() const {
    return vertices_.begin();
}
std::array<Vertex, 3>::const_iterator Triangle::end() const {
    return vertices_.end();
}

}  // namespace renderer