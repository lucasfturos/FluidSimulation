#include "render.hpp"

Render::Render()
    : window(nullptr), renderer(nullptr), texture(nullptr), surface(nullptr),
      quit(false), fluid(std::make_shared<Fluid>()),
      controlPanel(std::make_shared<ControlPanel>()) {}

Render::~Render() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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

float Render::scaleFactorX() const {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    return windowWidth / static_cast<float>(screenWidth);
}

float Render::scaleFactorY() const {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    return windowHeight / static_cast<float>(screenHeight);
}

void Render::run() {
    setup();
    controlPanel->setup();

#ifdef __EMSCRIPTEN__
    loop = [&]()
#else
    while (!quit)
#endif
    {
        frameStart = SDL_GetTicks();
        handleEvents();
        SDL_RenderClear(renderer);

        draw();

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    };
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, true);
#endif
}
