#include "vertex.h"

namespace renderer {

Vertex::Vertex(const Vector4& pos, const sf::Color& col) : position_(pos), color_(col) {
}

Vertex Vertex::interpolate(const Vertex& other, double t) const {
    Vector4 newPos = position_ + (other.position_ - position_) * t;

    sf::Color newColor(
        (color_.r + (other.color_.r - color_.r) * t), (color_.g + (other.color_.g - color_.g) * t),
        (color_.b + (other.color_.b - color_.b) * t), (color_.a + (other.color_.a - color_.a) * t));

    return Vertex(newPos, newColor);
}

const Vector4& Vertex::getPosition() const {
    return position_;
}
const sf::Color& Vertex::getColor() const {
    return color_;
}

}  // namespace renderer
