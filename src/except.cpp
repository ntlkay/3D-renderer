#include "except.h"

namespace except {
void react() {
    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
}
}  // namespace except
