#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>

#include "application.h"
#include "camera.h"
#include "object.h"
#include "renderer.h"
#include "screen.h"
#include "triangle.h"
#include "vector4.h"
#include "vertex.h"
#include "world.h"

int main() {
    try {
        renderer::Application app;
        app.run();
    } catch (std::exception& excep) {
        std::cerr << "Exception: " << excep.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
    return 0;
}