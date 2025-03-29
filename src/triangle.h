#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "vertex.h"

namespace renderer {

class Triangle {
private:
    std::array<Vertex, 3> vertices_;

public:
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

    Triangle transform(const glm::mat4& matrix) const;

    const Vertex& operator[](size_t index) const;

    const std::array<Vertex, 3>& getVertices() const;
    std::array<Vector4, 3> getPositions() const;

    std::array<Vertex, 3>::iterator begin();
    std::array<Vertex, 3>::iterator end();
    std::array<Vertex, 3>::const_iterator begin() const;
    std::array<Vertex, 3>::const_iterator end() const;
};

}  // namespace renderer
