#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace renderer {
namespace {

enum class LineType { Steep, Shallow };

struct LineDrawingState {
    int x0, y0;
    int x1, y1;
    LineType line_type;
};

LineDrawingState prepareLineState(const glm::ivec2& a, const glm::ivec2& b) {
    LineDrawingState state;
    state.x0 = a.x;
    state.y0 = a.y;
    state.x1 = b.x;
    state.y1 = b.y;

    state.line_type = LineType::Shallow;
    if (std::abs(state.x0 - state.x1) < std::abs(state.y0 - state.y1)) {
        std::swap(state.x0, state.y0);
        std::swap(state.x1, state.y1);
        state.line_type = LineType::Steep;
    }

    if (state.x0 > state.x1) {
        std::swap(state.x0, state.x1);
        std::swap(state.y0, state.y1);
    }

    return state;
}

glm::ivec2 toScreenCoords(const Vector4& v, const Screen& screen) {
    return {static_cast<int>((v.x() + 1.0) * 0.5 * screen.getWidth()),
            static_cast<int>((1.0 - v.y()) * 0.5 * screen.getHeight())};
}

}  // anonymous namespace

Renderer::Renderer() {
    ambient_light_ = sf::Color(50, 50, 50);
    addLight(Vector4(0.0f, 5.0f, 5.0f, 1.0f), sf::Color::White, 1.0);
}

void Renderer::addLight(const Vector4& position, const sf::Color& color, float intensity) {
    lights_.push_back({position, color, intensity});
}

void Renderer::setAmbientLight(const sf::Color& color) {
    ambient_light_ = color;
}

std::vector<Triangle> Renderer::handleAllVisible(const Triangle& triangle) const {
    return {triangle};
}

std::vector<Triangle> Renderer::handleNoneVisible() const {
    return {};
}

std::vector<Triangle> Renderer::handleOneVisible(const Triangle& triangle, float nearPlane,
                                                 const std::array<bool, 3>& is_visible) const {
    int front_idx = -1;
    int back_idx1 = -1;
    int back_idx2 = -1;

    for (int i = 0; i < 3; ++i) {
        if (is_visible[i]) {
            front_idx = i;
            back_idx1 = (i + 1) % 3;
            back_idx2 = (i + 2) % 3;
            break;
        }
    }

    Vertex front = triangle.GetVertex(front_idx);
    Vertex back1 = triangle.GetVertex(back_idx1);
    Vertex back2 = triangle.GetVertex(back_idx2);

    float t1 =
        (nearPlane - back1.getPosition().z()) / (front.getPosition().z() - back1.getPosition().z());
    float t2 =
        (nearPlane - back2.getPosition().z()) / (front.getPosition().z() - back2.getPosition().z());

    Vertex intersect_1 = back1.interpolate(front, t1);
    Vertex intersect_2 = back2.interpolate(front, t2);

    return {Triangle{front, intersect_1, intersect_2}};
}

std::vector<Triangle> Renderer::handleTwoVisible(const Triangle& triangle, float nearPlane,
                                                 const std::array<bool, 3>& is_visible) const {
    int back_idx1 = -1;
    int front_idx = -1;
    int back_idx2 = -1;

    for (int i = 0; i < 3; ++i) {
        if (!is_visible[i]) {
            back_idx1 = i;
            front_idx = (i + 1) % 3;
            front_idx = is_visible[front_idx] ? front_idx : (i + 2) % 3;
            back_idx2 = 3 - back_idx1 - front_idx;
            break;
        }
    }

    Vertex front_1 = triangle.GetVertex(front_idx);
    Vertex front_2 = triangle.GetVertex(back_idx2);
    Vertex back = triangle.GetVertex(back_idx1);

    float t1 =
        (nearPlane - back.getPosition().z()) / (front_1.getPosition().z() - back.getPosition().z());
    float t2 =
        (nearPlane - back.getPosition().z()) / (front_2.getPosition().z() - back.getPosition().z());

    Vertex intersect_1 = back.interpolate(front_1, t1);
    Vertex intersect_2 = back.interpolate(front_2, t2);

    return {
        Triangle{front_1, front_2, intersect_1},
        Triangle{front_2, intersect_1, intersect_2},
    };
}

std::vector<Triangle> Renderer::clipTriangle(const Triangle& triangle, float nearPlane) const {
    std::array<bool, 3> is_visible;
    for (int i = 0; i < 3; ++i) {
        is_visible[i] = (triangle.GetVertex(i).getPosition().z() >= nearPlane);
    }

    const int visible_count = std::count(is_visible.begin(), is_visible.end(), true);

    switch (visible_count) {
        case 0:
            return handleNoneVisible();
        case 1:
            return handleOneVisible(triangle, nearPlane, is_visible);
        case 2:
            return handleTwoVisible(triangle, nearPlane, is_visible);
        case 3:
            return handleAllVisible(triangle);
        default:
            return {};
    }
}

Triangle Renderer::transformTriangle(const Triangle& triangle, const Matrix4& matrix) const {
    Triangle copied_triangle = triangle;
    copied_triangle.transform(matrix);
    return copied_triangle;
}

Vector4 Renderer::get_edge(const Triangle& triangle, int from, int to) const {
    return triangle.GetVertex(to).getPosition() - triangle.GetVertex(from).getPosition();
}

Vector4 Renderer::cross_product(const Vector4& a, const Vector4& b) const {
    return Vector4(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                   a.x() * b.y() - a.y() * b.x(), 0.0f);
}

sf::Color Renderer::apply_ambient_light(const sf::Color& base_color) const {
    return sf::Color(static_cast<uint8_t>(ambient_light_.r * base_color.r / 255),
                     static_cast<uint8_t>(ambient_light_.g * base_color.g / 255),
                     static_cast<uint8_t>(ambient_light_.b * base_color.b / 255), 255);
}

Vector4 Renderer::light_direction(const Vector4& position, const Light& light) const {
    return (light.position - position).normalize();
}

float Renderer::dot3(const Vector4& a, const Vector4& b) const {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

float Renderer::diffuse_intensity(const Vector4& normal, const Vector4& light_dir) const {
    return std::max(0.0f, dot3(normal, light_dir));
}

sf::Color Renderer::accumulate_diffuse(const sf::Color& current, const sf::Color& base_color,
                                       float factor) const {
    sf::Color out = current;
    auto add_channel = [factor](uint8_t cur, uint8_t base) {
        int v = cur + static_cast<int>(base * factor);
        return static_cast<uint8_t>(v > 255 ? 255 : v);
    };
    out.r = add_channel(out.r, base_color.r);
    out.g = add_channel(out.g, base_color.g);
    out.b = add_channel(out.b, base_color.b);
    return out;
}

Vector4 Renderer::calculateNormal(const Triangle& triangle) const {
    Vector4 edge1 = get_edge(triangle, 0, 1);
    Vector4 edge2 = get_edge(triangle, 0, 2);
    return cross_product(edge1, edge2);
}

sf::Color Renderer::calculateLighting(const Vector4& position, const Vector4& normal,
                                      const sf::Color& base_color) const {
    sf::Color shaded = apply_ambient_light(base_color);
    for (const auto& light : lights_) {
        Vector4 dir = light_direction(position, light);
        float diff = diffuse_intensity(normal, dir) * light.intensity;
        if (diff > 0.0f)
            shaded = accumulate_diffuse(shaded, base_color, diff);
    }
    return shaded;
}

Renderer::screen_triangle_data Renderer::prepare_screen_triangle(const Triangle& triangle,
                                                                 Screen& screen) const {
    screen_triangle_data data;
    data.world_vertex0 = triangle.GetVertex(0).getPosition().toVector3();
    data.world_vertex1 = triangle.GetVertex(1).getPosition().toVector3();
    data.world_vertex2 = triangle.GetVertex(2).getPosition().toVector3();

    auto to_screen = [&screen](const Vector3& position) {
        return glm::vec2((position.x + 1.0f) * 0.5f * screen.getWidth(),
                         (1.0f - position.y) * 0.5f * screen.getHeight());
    };

    data.screen_vertex0 = to_screen(data.world_vertex0);
    data.screen_vertex1 = to_screen(data.world_vertex1);
    data.screen_vertex2 = to_screen(data.world_vertex2);

    float min_x = std::min({data.screen_vertex0.x, data.screen_vertex1.x, data.screen_vertex2.x});
    float max_x = std::max({data.screen_vertex0.x, data.screen_vertex1.x, data.screen_vertex2.x});
    float min_y = std::min({data.screen_vertex0.y, data.screen_vertex1.y, data.screen_vertex2.y});
    float max_y = std::max({data.screen_vertex0.y, data.screen_vertex1.y, data.screen_vertex2.y});

    data.bbox_start_x = std::max(0, static_cast<int>(std::floor(min_x)));
    data.bbox_end_x =
        std::min(static_cast<int>(screen.getWidth()) - 1, static_cast<int>(std::ceil(max_x)));
    data.bbox_start_y = std::max(0, static_cast<int>(std::floor(min_y)));
    data.bbox_end_y =
        std::min(static_cast<int>(screen.getHeight()) - 1, static_cast<int>(std::ceil(max_y)));

    data.triangle_area = (data.screen_vertex1.y - data.screen_vertex2.y) *
                             (data.screen_vertex0.x - data.screen_vertex2.x) +
                         (data.screen_vertex2.x - data.screen_vertex1.x) *
                             (data.screen_vertex0.y - data.screen_vertex2.y);

    data.face_normal = calculateNormal(triangle);
    return data;
}

bool Renderer::compute_barycentric(const screen_triangle_data& data, int x, int y, float& alpha,
                                   float& beta, float& gamma) const {
    if (data.triangle_area == 0.0f)
        return false;

    glm::vec2 pixel_center(x + 0.5f, y + 0.5f);

    alpha = ((data.screen_vertex1.y - data.screen_vertex2.y) *
                 (pixel_center.x - data.screen_vertex2.x) +
             (data.screen_vertex2.x - data.screen_vertex1.x) *
                 (pixel_center.y - data.screen_vertex2.y)) /
            data.triangle_area;

    beta = ((data.screen_vertex2.y - data.screen_vertex0.y) *
                (pixel_center.x - data.screen_vertex2.x) +
            (data.screen_vertex0.x - data.screen_vertex2.x) *
                (pixel_center.y - data.screen_vertex2.y)) /
           data.triangle_area;

    gamma = 1.0f - alpha - beta;

    return (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f);
}

Vector4 Renderer::interpolate_position(const screen_triangle_data& data, float alpha, float beta,
                                       float gamma) const {
    return Vector4(
        alpha * data.world_vertex0.x + beta * data.world_vertex1.x + gamma * data.world_vertex2.x,
        alpha * data.world_vertex0.y + beta * data.world_vertex1.y + gamma * data.world_vertex2.y,
        alpha * data.world_vertex0.z + beta * data.world_vertex1.z + gamma * data.world_vertex2.z,
        1.0f);
}

Vector4 Renderer::interpolate_normal(const Triangle& triangle, const screen_triangle_data& data,
                                     float alpha, float beta, float gamma) const {
    const auto& normal0 = triangle.GetVertex(0).getNormal();
    const auto& normal1 = triangle.GetVertex(1).getNormal();
    const auto& normal2 = triangle.GetVertex(2).getNormal();
    Vector4 zero(0, 0, 0, 0);

    if (normal0 != zero && normal1 != zero && normal2 != zero) {
        return Vector4(alpha * normal0.x() + beta * normal1.x() + gamma * normal2.x(),
                       alpha * normal0.y() + beta * normal1.y() + gamma * normal2.y(),
                       alpha * normal0.z() + beta * normal1.z() + gamma * normal2.z(), 0.0f)
            .normalize();
    }
    return data.face_normal;
}

sf::Color Renderer::interpolate_color(const Triangle& triangle, float alpha, float beta,
                                      float gamma) const {
    const auto& color0 = triangle.GetVertex(0).getColor();
    const auto& color1 = triangle.GetVertex(1).getColor();
    const auto& color2 = triangle.GetVertex(2).getColor();
    sf::Color color;
    color.r = static_cast<uint8_t>(alpha * color0.r + beta * color1.r + gamma * color2.r);
    color.g = static_cast<uint8_t>(alpha * color0.g + beta * color1.g + gamma * color2.g);
    color.b = static_cast<uint8_t>(alpha * color0.b + beta * color1.b + gamma * color2.b);
    color.a = 255;
    return color;
}

void Renderer::drawTriangle(const Triangle& triangle, Screen& screen) const {
    auto data = prepare_screen_triangle(triangle, screen);
    if (data.triangle_area == 0.0f)
        return;

    for (int y = data.bbox_start_y; y <= data.bbox_end_y; ++y) {
        for (int x = data.bbox_start_x; x <= data.bbox_end_x; ++x) {
            float alpha, beta, gamma;
            if (!compute_barycentric(data, x, y, alpha, beta, gamma))
                continue;

            float z = alpha * data.world_vertex0.z + beta * data.world_vertex1.z +
                      gamma * data.world_vertex2.z;
            Vector4 position = interpolate_position(data, alpha, beta, gamma);
            Vector4 normal = interpolate_normal(triangle, data, alpha, beta, gamma);
            sf::Color base_color = interpolate_color(triangle, alpha, beta, gamma);

            sf::Color shaded = calculateLighting(position, normal, base_color);
            screen.setPixel(x, y, z, shaded);
        }
    }
}

Screen Renderer::render(const World& world, const Camera& camera, Screen&& screen) {
    screen.fill(sf::Color::Black);

    for (const auto& triangle : world) {
        auto transformed = transformTriangle(triangle, camera.getWorldToClip());
        auto clipped = clipTriangle(transformed, camera.getNear());
        for (const auto& clipped_triangle : clipped) {
            drawTriangle(clipped_triangle, screen);
        }
    }

    return std::move(screen);
}

}  // namespace renderer
