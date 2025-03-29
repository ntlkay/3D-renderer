#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <type_traits>

namespace renderer {

class Vector4 : public glm::dvec4 {

public:
    Vector4();
    Vector4(double x, double y, double z, double w);
    explicit Vector4(const glm::dvec4& v);

    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(double alpha) const;
    Vector4 operator/(double alpha) const;

    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    Vector4 normalize() const;
    double length() const;
};

}  // namespace renderer
