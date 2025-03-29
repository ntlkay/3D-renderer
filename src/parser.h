#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>

#include "triangle.h"
#include "vertex.h"

namespace renderer {

class CubeCreator {
public:
    static std::vector<Triangle> create();
};

}  // namespace renderer
