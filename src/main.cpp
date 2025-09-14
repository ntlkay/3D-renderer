#include "application.h"
#include "except.h"

int main() {
    try {
        renderer::Application app;
        app.run();
    } catch (...) {
        except::react();
    }
    return 0;
}
