#pragma once

#include <algorithm>
#include <cmath>

#include "camera.h"
#include "screen.h"
#include "triangle.h"
#include "world.h"

namespace renderer {

class Renderer {
private:
    void drawTriangle(const Triangle& triangle, Screen& screen) const;

public:
    void render(const World& world, const Camera& camera, Screen& screen);
};

}  // namespace renderer
