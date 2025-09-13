#include "screen.h"

namespace renderer {

float& Screen::zBuffer(unsigned int x, unsigned int y) {
    return z_buffer_[y * width_ + x];
}

float Screen::zBuffer(unsigned int x, unsigned int y) const {
    return z_buffer_[y * width_ + x];
}

Screen::Screen(Width width, Height height)
    : width_(width), pixels_(width * height), z_buffer_(width * height, kBufferDefaultValue) {
    for (int i = 0; i < width * height; ++i) {
        pixels_[i].position = sf::Vector2f(i % width, i / width);
        pixels_[i].color = sf::Color::Black;
    }
}

void Screen::fill(const sf::Color& color) {
    std::fill(z_buffer_.begin(), z_buffer_.end(), kBufferDefaultValue);
    for (auto& pixel : pixels_) {
        pixel.color = color;
    }
}

void Screen::setPixel(int x, int y, double z, const sf::Color& color) {
    if (x >= width_ || y >= getHeight()) {
        return;
    }

    if (z < zBuffer(x, y)) {
        zBuffer(x, y) = z;
        pixels_[y * width_ + x].color = color;
    }
}

void Screen::draw(sf::RenderWindow& window) const {
    window.draw(pixels_.data(), pixels_.size(), sf::PrimitiveType::Points);
}

const std::vector<sf::Vertex>& Screen::getPixels() const {
    return pixels_;
}

int Screen::getWidth() const {
    return width_;
}

int Screen::getHeight() const {
    if (width_ == 0) {
        return 0;
    }
    return pixels_.size() / width_;
}

}  // namespace renderer
