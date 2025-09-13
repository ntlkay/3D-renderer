#include "parser.h"

#include <fstream>
#include <sstream>

namespace renderer {

Object CubeCreator::create(float size, const Matrix4& translation) {
    const float half_size = size * 0.5f;
    std::array<glm::vec4, 8> vertices{glm::vec4(-half_size, -half_size, -half_size, 1.0),
                                      glm::vec4(-half_size, -half_size, half_size, 1.0),
                                      glm::vec4(-half_size, half_size, -half_size, 1.0),
                                      glm::vec4(-half_size, half_size, half_size, 1.0),
                                      glm::vec4(half_size, -half_size, -half_size, 1.0),
                                      glm::vec4(half_size, -half_size, half_size, 1.0),
                                      glm::vec4(half_size, half_size, -half_size, 1.0),
                                      glm::vec4(half_size, half_size, half_size, 1.0)};

    for (auto& vertex : vertices)
        vertex = translation * vertex;

    struct Face {
        int a, b, c, d;
        sf::Color color;
    };
    static const Face faces[] = {
        {1, 5, 7, 3, sf::Color(255, 0, 0)},   {0, 4, 6, 2, sf::Color(0, 255, 0)},
        {2, 6, 7, 3, sf::Color(0, 0, 255)},   {0, 1, 5, 4, sf::Color(255, 255, 0)},
        {0, 2, 3, 1, sf::Color(0, 255, 255)}, {4, 5, 7, 6, sf::Color(255, 0, 255)}};

    std::vector<Triangle> triangles;
    triangles.reserve(12);

    auto addFace = [&](const Face& face) {
        auto make_vertex = [&](int idx) { return Vertex(Vector4(vertices[idx]), face.color); };
        Vertex v0 = make_vertex(face.a);
        Vertex v1 = make_vertex(face.b);
        Vertex v2 = make_vertex(face.c);
        Vertex v3 = make_vertex(face.d);
        triangles.emplace_back(v0, v1, v2);
        triangles.emplace_back(v0, v2, v3);
    };

    for (const auto& face : faces)
        addFace(face);

    return Object(std::move(triangles));
}

void ObjParser::parseVertex(std::istringstream& iss, std::vector<Vector4>& vertices) {
    float x, y, z;
    iss >> x >> y >> z;
    vertices.emplace_back(x, y, z, 1.0f);
}

void ObjParser::parseNormal(std::istringstream& iss, std::vector<Vector4>& normals) {
    float normal_x, normal_y, normal_z;
    iss >> normal_x >> normal_y >> normal_z;
    normals.emplace_back(normal_x, normal_y, normal_z, 0.0f);
}

std::vector<ObjParser::FaceElement> ObjParser::readFaceElements(std::istringstream& iss) {
    std::vector<FaceElement> elements;
    std::string token;
    while (iss >> token) {
        FaceElement face_element;
        std::istringstream part(token);
        std::string vertex_string, texture_string, normal_string;
        std::getline(part, vertex_string, '/');
        std::getline(part, texture_string, '/');
        std::getline(part, normal_string, '/');
        if (!vertex_string.empty())
            face_element.vertex_index = std::stoi(vertex_string) - 1;
        if (!normal_string.empty()) {
            face_element.normal_index = std::stoi(normal_string) - 1;
            face_element.has_normal = true;
        }
        elements.push_back(face_element);
    }
    return elements;
}

Vector4 ObjParser::computeFaceNormal(const std::vector<Vector4>& vertices,
                                     const std::vector<FaceElement>& elements) {
    if (elements.size() < 3)
        return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    const auto& vertex_A = vertices[elements[0].vertex_index].vec;
    const auto& vertex_B = vertices[elements[1].vertex_index].vec;
    const auto& vertex_C = vertices[elements[2].vertex_index].vec;
    glm::vec3 edge_vector_AB = glm::vec3(vertex_B - vertex_A);
    glm::vec3 edge_vector_AC = glm::vec3(vertex_C - vertex_A);
    glm::vec3 face_normal = glm::normalize(glm::cross(edge_vector_AB, edge_vector_AC));
    return Vector4(glm::vec4(face_normal, 0.0f));
}

void ObjParser::triangulateFace(const std::vector<FaceElement>& elements,
                                const std::vector<Vector4>& vertices,
                                const std::vector<Vector4>& normals, const sf::Color& color,
                                std::vector<Triangle>& out_triangles) {
    if (elements.size() < 3)
        return;

    bool all_have_normals =
        std::all_of(elements.begin(), elements.end(),
                    [](const FaceElement& face_element) { return face_element.has_normal; });
    Vector4 face_normal;
    if (!all_have_normals)
        face_normal = computeFaceNormal(vertices, elements);

    auto fetchNormal = [&](const FaceElement& face_element) -> Vector4 {
        if (all_have_normals && face_element.normal_index >= 0 &&
            face_element.normal_index < static_cast<int>(normals.size()))
            return normals[face_element.normal_index];
        return face_normal;
    };

    auto makeVertex = [&](const FaceElement& face_element) -> Vertex {
        return Vertex(vertices[face_element.vertex_index], fetchNormal(face_element), color);
    };

    for (size_t i = 1; i + 1 < elements.size(); ++i) {
        Vertex vertex_0 = makeVertex(elements[0]);
        Vertex vertex_1 = makeVertex(elements[i]);
        Vertex vertex_2 = makeVertex(elements[i + 1]);
        out_triangles.emplace_back(vertex_0, vertex_1, vertex_2);
    }
}

void ObjParser::parseFace(std::istringstream& iss, const std::vector<Vector4>& vertices,
                          const std::vector<Vector4>& normals, const sf::Color& color,
                          std::vector<Triangle>& out_triangles) {
    auto elements = readFaceElements(iss);
    triangulateFace(elements, vertices, normals, color, out_triangles);
}

Object ObjParser::parse(const std::string& file_path, const sf::Color& color) {
    std::ifstream file(file_path);
    if (!file.is_open())
        return Object({});

    std::vector<Vector4> vertices;
    std::vector<Vector4> normals;
    std::vector<Triangle> triangles;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            parseVertex(iss, vertices);
        } else if (type == "vn") {
            parseNormal(iss, normals);
        } else if (type == "f") {
            parseFace(iss, vertices, normals, color, triangles);
        }
    }

    return Object(std::move(triangles));
}

}  // namespace renderer
