#include "Render/render.hpp"
#include <iostream>

int main(void) {
    try {
        auto render = std::make_shared<Render>();
        render->run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}