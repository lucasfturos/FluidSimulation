#include "render.hpp"

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
