#include "screen.h"

namespace renderer {

Screen::Screen(unsigned int w, unsigned int h) : width_(w), height_(h) {
    pixels_.resize(width_ * height_);
    z_buffer_.resize(width_ * height_, std::numeric_limits<double>::max());
    clear();
}

void Screen::clear() {
    std::fill(pixels_.begin(), pixels_.end(), sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black));
    std::fill(z_buffer_.begin(), z_buffer_.end(), std::numeric_limits<double>::max());
}

void Screen::setPixel(unsigned int x, unsigned int y, double z, const sf::Color& color) {
    if (x >= width_ || y >= height_)
        return;

    const size_t index = y * width_ + x;
    if (z < z_buffer_[index]) {
        z_buffer_[index] = z;
        pixels_[index] = sf::Vertex(sf::Vector2f(x, y), color);
    }
}

const std::vector<sf::Vertex>& Screen::getPixels() const {
    return pixels_;
}

unsigned int Screen::getWidth() const {
    return width_;
}

unsigned int Screen::getHeight() const {
    return height_;
}

}  // namespace renderer
