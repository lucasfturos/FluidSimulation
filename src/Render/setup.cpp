#include "render.hpp"

void Render::setupWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL: " +
                                 std::string(SDL_GetError()));

    try {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                  screenHeight, SDL_WINDOW_OPENGL);
        if (!window)
            throw std::runtime_error("Error creating window: " +
                                     std::string(SDL_GetError()));

        renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
            throw std::runtime_error("Error creating renderer: " +
                                     std::string(SDL_GetError()));

        surface =
            SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0x00FF0000,
                                 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!surface)
            throw std::runtime_error("SDL_CreateRGBSurface Error: " +
                                     std::string(SDL_GetError()));

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING, screenWidth,
                                    screenHeight);
        if (!texture)
            throw std::runtime_error("SDL_CreateTexture Error: " +
                                     std::string(SDL_GetError()));

    } catch (const std::runtime_error &e) {
        destroyWindow();
        throw;
    }
}

void Render::destroyWindow() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (surface) {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}