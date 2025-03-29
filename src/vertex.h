#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp>

#include "vector4.h"

namespace renderer {

class Vertex {
private:
    Vector4 position_;
    sf::Color color_;

public:
    Vertex() = default;
    Vertex(const Vector4& pos, const sf::Color& col);

    Vertex interpolate(const Vertex& other, double t) const;

    const Vector4& getPosition() const;
    const sf::Color& getColor() const;
};

}  // namespace renderer
