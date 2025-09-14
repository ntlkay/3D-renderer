#pragma once

#include <algorithm>
#include <limits>
#include <SFML/Graphics.hpp>

namespace renderer {

enum Width : int;
enum Height : int;

class Screen {
private:
    static constexpr float kBufferDefaultValue = std::numeric_limits<float>::max();

    int width_;
    std::vector<sf::Vertex> pixels_;
    std::vector<float> z_buffer_;

    float& zBuffer(unsigned int x, unsigned int y);
    float zBuffer(unsigned int x, unsigned int y) const;

public:
    Screen(Width width, Height height);

    void fill(const sf::Color& color);
    void setPixel(int x, int y, double z, const sf::Color& color);
    void draw(sf::RenderWindow& window) const;

    const std::vector<sf::Vertex>& getPixels() const;

    int getWidth() const;
    int getHeight() const;
};

}  // namespace renderer
