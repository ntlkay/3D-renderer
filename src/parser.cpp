#include "parser.h"

namespace renderer {

std::vector<Triangle> CubeCreator::create() {
    std::vector<Triangle> triangles;

    const std::vector<Vector4> vertices = {
        Vector4(-1, -1, -1, 1), Vector4(1, -1, -1, 1), Vector4(1, 1, -1, 1), Vector4(-1, 1, -1, 1),
        Vector4(-1, -1, 1, 1),  Vector4(1, -1, 1, 1),  Vector4(1, 1, 1, 1),  Vector4(-1, 1, 1, 1)};

    const std::vector<std::tuple<int, int, int>> faces = {
        {0, 1, 2}, {2, 3, 0}, {1, 5, 6}, {6, 2, 1}, {5, 4, 7}, {7, 6, 5},
        {4, 0, 3}, {3, 7, 4}, {3, 2, 6}, {6, 7, 3}, {4, 5, 1}, {1, 0, 4}};

    const std::vector<sf::Color> colors = {sf::Color::Red,    sf::Color::Green, sf::Color::Blue,
                                           sf::Color::Yellow, sf::Color::Cyan,  sf::Color::Magenta};

    for (size_t i = 0; i < faces.size(); ++i) {
        const auto [a, b, c] = faces[i];
        triangles.emplace_back(Vertex(vertices[a], colors[i / 2]),
                               Vertex(vertices[b], colors[i / 2]),
                               Vertex(vertices[c], colors[i / 2]));
    }

    return triangles;
}

}  // namespace renderer