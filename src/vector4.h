#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <type_traits>

#include "vector3.h"

namespace renderer {

struct Vector4 {

    glm::vec4 vec;
    Vector4();
    Vector4(float x, float y, float z, float w);
    explicit Vector4(const glm::vec4& vec);

    operator const glm::vec4&() const;
    operator glm::vec4() const;

    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(float alpha) const;
    Vector4 operator/(float alpha) const;

    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    Vector4 normalize() const;
    Vector3 toVector3() const;
    float length() const;

    float x() const;
    float y() const;
    float z() const;
    float w() const;
};

}  // namespace renderer
