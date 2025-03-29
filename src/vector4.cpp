#include "vector4.h"

namespace renderer {

Vector4::Vector4() : glm::dvec4(0.0, 0.0, 0.0, 1.0) {
}

Vector4::Vector4(double x, double y, double z, double w) : glm::dvec4(x, y, z, w) {
}

Vector4::Vector4(const glm::dvec4& v) : glm::dvec4(v) {
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(double alpha) const {
    return Vector4(x * alpha, y * alpha, z * alpha, w * alpha);
}

Vector4 Vector4::operator/(double alpha) const {
    return Vector4(x / alpha, y / alpha, z / alpha, w / alpha);
}

bool Vector4::operator==(const Vector4& other) const {
    return glm::all(glm::equal(*this, other));
}

bool Vector4::operator!=(const Vector4& other) const {
    return !(*this == other);
}

Vector4 Vector4::normalize() const {
    if (w == 0.0)
        return *this;
    return Vector4(x / w, y / w, z / w, 1.0);
}

double Vector4::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

}  // namespace renderer
