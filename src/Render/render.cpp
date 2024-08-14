#include "render.hpp"
#include "../../external/imgui/backends/imgui_impl_sdl2.h"
#include "../../external/imgui/backends/imgui_impl_sdlrenderer2.h"

Render::Render()
    : window(nullptr), renderer(nullptr), texture(nullptr), surface(nullptr),
      quit(false), fluid(std::make_shared<Fluid>()),
      controlPanel(std::make_shared<ControlPanel>()) {
    setupWindow();
    setupImGui();
    fluid->setSurface(surface);
}

Render::~Render() { destroyWindow(); }

void Render::draw() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    controlPanel->run();

    if (SDL_LockSurface(surface) == 0) {
        SDL_memset(surface->pixels, 0, surface->h * surface->pitch);

        fluid->setSimulationParams(controlPanel->getSimulationParams());
        fluid->draw();

        SDL_UnlockSurface(surface);
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void Render::run() {
    controlPanel->setup();

    while (!quit) {
        frameStart = SDL_GetTicks();
        handleEvents();
        SDL_RenderClear(renderer);

        draw();

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}