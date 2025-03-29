#pragma once

#include <algorithm>
#include <limits>
#include <SFML/Graphics.hpp>

namespace renderer {

class Screen {
private:
    unsigned int width_;
    unsigned int height_;
    std::vector<sf::Vertex> pixels_;
    std::vector<float> z_buffer_;

public:
    Screen(unsigned int w, unsigned int h);

    void clear();
    void setPixel(unsigned int x, unsigned int y, double z, const sf::Color& color);

    const std::vector<sf::Vertex>& getPixels() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
};

}  // namespace renderer