#include "vertex.h"

namespace renderer {

Vertex::Vertex() : position_(0, 0, 0, 1), normal_(0, 0, 0, 0), color_(sf::Color::White) {
}

Vertex::Vertex(const Vector4& pos, const sf::Color& col)
    : position_(pos), normal_(Vector4(0, 0, 0, 0)), color_(col) {
}

Vertex::Vertex(const Vector4& pos, const Vector4& normal, const sf::Color& col)
    : position_(pos), normal_(normal), color_(col) {
}

inline sf::Color interpolateColor(const sf::Color& c1, const sf::Color& c2, float t) {
    auto interpolation = [t](uint8_t a, uint8_t b) {
        return static_cast<uint8_t>(a + (b - a) * t);
    };
    return sf::Color(interpolation(c1.r, c2.r), interpolation(c1.g, c2.g),
                     interpolation(c1.b, c2.b), interpolation(c1.a, c2.a));
}

Vertex Vertex::interpolate(const Vertex& other, float t) const {
    Vector4 newPos = position_ + (other.position_ - position_) * static_cast<float>(t);
    Vector4 newNormal = normal_ + (other.normal_ - normal_) * static_cast<float>(t);
    sf::Color newColor = interpolateColor(color_, other.color_, t);
    return Vertex(newPos, newNormal, newColor);
}

void Vertex::transform(const Matrix4& matrix) {
    position_ = Vector4{matrix * position_};
    normal_ = Vector4{matrix * normal_};
}

const Vector4& Vertex::getPosition() const {
    return position_;
}

const Vector4& Vertex::getNormal() const {
    return normal_;
}

const sf::Color& Vertex::getColor() const {
    return color_;
}

void Vertex::setNormal(const Vector4& normal) {
    normal_ = normal;
}

}  // namespace renderer
