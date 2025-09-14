#pragma once

#include <algorithm>
#include <cmath>

#include "camera.h"
#include "linear_algebra.h"
#include "screen.h"
#include "triangle.h"
#include "vector4.h"
#include "vertex.h"
#include "world.h"

namespace renderer {

class Renderer {
private:
    struct Light {
        Vector4 position;
        sf::Color color;
        float intensity;
    };

    struct screen_triangle_data {
        glm::vec2 screen_vertex0, screen_vertex1, screen_vertex2;
        Vector3 world_vertex0, world_vertex1, world_vertex2;
        float triangle_area;
        int bbox_start_x, bbox_end_x, bbox_start_y, bbox_end_y;
        Vector4 face_normal;
    };

    std::vector<Light> lights_;
    sf::Color ambient_light_;

    void drawTriangle(const Triangle& triangle, Screen& screen) const;
    Vector4 get_edge(const Triangle& triangle, int from, int to) const;
    Vector4 cross_product(const Vector4& a, const Vector4& b) const;

    sf::Color apply_ambient_light(const sf::Color& base_color) const;
    Vector4 light_direction(const Vector4& position, const Light& light) const;
    float dot3(const Vector4& a, const Vector4& b) const;
    float diffuse_intensity(const Vector4& normal, const Vector4& light_dir) const;
    sf::Color accumulate_diffuse(const sf::Color& current, const sf::Color& base_color,
                                 float factor) const;

    Vector4 calculateNormal(const Triangle& triangle) const;
    sf::Color calculateLighting(const Vector4& position, const Vector4& normal,
                                const sf::Color& base_color) const;
    float edgeFunction(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const;

    Triangle transformTriangle(const Triangle& triangle, const Matrix4& matrix) const;
    std::vector<Triangle> clipTriangle(const Triangle& triangle, float nearPlane) const;
    std::vector<Triangle> handleAllVisible(const Triangle& triangle) const;
    std::vector<Triangle> handleNoneVisible() const;
    std::vector<Triangle> handleOneVisible(const Triangle& triangle, float nearPlane,
                                           const std::array<bool, 3>& is_visible) const;
    std::vector<Triangle> handleTwoVisible(const Triangle& triangle, float nearPlane,
                                           const std::array<bool, 3>& is_visible) const;

    screen_triangle_data prepare_screen_triangle(const Triangle& triangle, Screen& screen) const;
    bool compute_barycentric(const screen_triangle_data& data, int x, int y, float& alpha,
                             float& beta, float& gamma) const;
    Vector4 interpolate_position(const screen_triangle_data& data, float alpha, float beta,
                                 float gamma) const;
    Vector4 interpolate_normal(const Triangle& triangle, const screen_triangle_data& data,
                               float alpha, float beta, float gamma) const;
    sf::Color interpolate_color(const Triangle& triangle, float alpha, float beta,
                                float gamma) const;

public:
    Renderer();
    void addLight(const Vector4& position, const sf::Color& color, float intensity);
    void setAmbientLight(const sf::Color& color);
    Screen render(const World& world, const Camera& camera, Screen&& screen);
};

}  // namespace renderer
