#include "renderer.h"

namespace renderer {
namespace triangle_rendering {

class LineDrawingState {
public:
    int x0, y0;
    int x1, y1;
    bool steep;
};

LineDrawingState prepareLineState(const glm::ivec2& a, const glm::ivec2& b) {
    LineDrawingState state;
    state.x0 = a.x;
    state.y0 = a.y;
    state.x1 = b.x;
    state.y1 = b.y;

    state.steep = false;
    if (std::abs(state.x0 - state.x1) < std::abs(state.y0 - state.y1)) {
        std::swap(state.x0, state.y0);
        std::swap(state.x1, state.y1);
        state.steep = true;
    }

    if (state.x0 > state.x1) {
        std::swap(state.x0, state.x1);
        std::swap(state.y0, state.y1);
    }

    return state;
}

void drawLineImpl(const LineDrawingState& state, Screen& screen, const sf::Color& color) {
    const int dx = state.x1 - state.x0;
    const int dy = state.y1 - state.y0;
    const int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = state.y0;

    for (int x = state.x0; x <= state.x1; ++x) {
        if (state.steep) {
            screen.setPixel(y, x, 0, color);
        } else {
            screen.setPixel(x, y, 0, color);
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (state.y1 > state.y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

glm::ivec2 toScreenCoords(const Vector4& v, const Screen& screen) {
    return {static_cast<int>((v.x + 1.0) * 0.5 * screen.getWidth()),
            static_cast<int>((1.0 - v.y) * 0.5 * screen.getHeight())};
}

}  // namespace triangle_rendering

void Renderer::drawTriangle(const Triangle& triangle, Screen& screen) const {
    const auto p0 =
        triangle_rendering::toScreenCoords(triangle[0].getPosition().normalize(), screen);
    const auto p1 =
        triangle_rendering::toScreenCoords(triangle[1].getPosition().normalize(), screen);
    const auto p2 =
        triangle_rendering::toScreenCoords(triangle[2].getPosition().normalize(), screen);

    triangle_rendering::drawLineImpl(triangle_rendering::prepareLineState(p0, p1), screen,
                                     triangle[0].getColor());

    triangle_rendering::drawLineImpl(triangle_rendering::prepareLineState(p1, p2), screen,
                                     triangle[1].getColor());

    triangle_rendering::drawLineImpl(triangle_rendering::prepareLineState(p2, p0), screen,
                                     triangle[2].getColor());
}

void Renderer::render(const World& world, const Camera& camera, Screen& screen) {
    screen.clear();

    for (const auto& triangle : world) {
        auto transformed = triangle.transform(camera.getWorldToClip());
        drawTriangle(transformed, screen);
    }
}

}  // namespace renderer