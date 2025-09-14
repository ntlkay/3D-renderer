#pragma once

#include <string>

#include "linear_algebra.h"
#include "object.h"

namespace renderer {

class CubeCreator {
public:
    static Object create(float size = 1.0, const Matrix4& translation = Matrix4(1.0f));
};

class ObjParser {
public:
    static Object parse(const std::string& filepath,
                        const sf::Color& defaultColor = sf::Color::White);

private:
    struct FaceElement {
        int vertex_index = -1;
        int normal_index = -1;
        bool has_normal = false;
    };

    static void parseVertex(std::istringstream& iss, std::vector<Vector4>& vertices);
    static void parseNormal(std::istringstream& iss, std::vector<Vector4>& normals);
    static void parseFace(std::istringstream& iss, const std::vector<Vector4>& vertices,
                          const std::vector<Vector4>& normals, const sf::Color& defaultColor,
                          std::vector<Triangle>& outTriangles);

    static std::vector<FaceElement> readFaceElements(std::istringstream& iss);
    static Vector4 computeFaceNormal(const std::vector<Vector4>& vertices,
                                     const std::vector<FaceElement>& elems);
    static void triangulateFace(const std::vector<FaceElement>& elems,
                                const std::vector<Vector4>& vertices,
                                const std::vector<Vector4>& normals, const sf::Color& color,
                                std::vector<Triangle>& outTriangles);
};

}  // namespace renderer
