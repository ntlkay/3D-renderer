#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "linear_algebra.h"
#include "vertex.h"

namespace renderer {

using iterator = std::array<Vertex, 3>::const_iterator;

class Triangle {
private:
    std::array<Vertex, 3> vertices_;

public:
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

    void transform(const Matrix4& matrix);

    const Vertex& GetVertex(unsigned int index) const;

    const std::array<Vertex, 3>& getVertices() const;
    std::array<Vector4, 3> getPositions() const;
    std::array<Vector4, 3> getNormals() const;

    iterator begin();
    iterator end();
    iterator begin() const;
    iterator end() const;
};

}  // namespace renderer
