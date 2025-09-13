#pragma once

#include <SFML/Graphics/Color.hpp>

#include "linear_algebra.h"
#include "vector4.h"

namespace renderer {

class Vertex {
private:
    Vector4 position_;
    Vector4 normal_;
    sf::Color color_;

public:
    Vertex();
    Vertex(const Vector4& pos, const sf::Color& col);
    Vertex(const Vector4& pos, const Vector4& normal, const sf::Color& col);

    Vertex interpolate(const Vertex& other, float t) const;
    void transform(const Matrix4& matrix);

    const Vector4& getPosition() const;
    const Vector4& getNormal() const;
    const sf::Color& getColor() const;

    void setNormal(const Vector4& normal);
};

}  // namespace renderer
