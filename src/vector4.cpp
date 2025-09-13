#include "vector4.h"

namespace renderer {

Vector4::Vector4() : vec(0.0f, 0.0f, 0.0f, 1.0f) {
}

Vector4::Vector4(float x, float y, float z, float w) : vec(x, y, z, w) {
}

Vector4::Vector4(const glm::vec4& vec) : vec(vec) {
}

Vector4::operator const glm::vec4&() const {
    return vec;
}

Vector4::operator glm::vec4() const {
    return vec;
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(vec + other.vec);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(vec - other.vec);
}

Vector4 Vector4::operator*(float alpha) const {
    return Vector4(vec * alpha);
}

Vector4 Vector4::operator/(float alpha) const {
    return Vector4(vec / alpha);
}

bool Vector4::operator==(const Vector4& other) const {
    return glm::all(glm::equal(vec, other.vec));
}

bool Vector4::operator!=(const Vector4& other) const {
    return !(*this == other);
}

Vector4 Vector4::normalize() const {
    if (vec.w == 0.0f)
        return *this;
    return Vector4(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w, 1.0f);
}

Vector3 Vector4::toVector3() const {
    if (vec.w != 0.0f)
        return Vector3{vec.x / vec.w, vec.y / vec.w, vec.z / vec.w};
    else
        return Vector3{vec.x, vec.y, vec.z};
}

float Vector4::length() const {
    return glm::length(glm::vec3(vec));
}

float Vector4::x() const {
    return vec.x;
}
float Vector4::y() const {
    return vec.y;
}
float Vector4::z() const {
    return vec.z;
}
float Vector4::w() const {
    return vec.w;
}

}  // namespace renderer
