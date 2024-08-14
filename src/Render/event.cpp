#include "../../external/imgui/backends/imgui_impl_sdl2.h"
#include "render.hpp"

void Render::handleEvents() {
    static int lastMouseX = 0;
    static int lastMouseY = 0;
    static bool mouseDown = false;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseDown = true;
                SDL_GetMouseState(&lastMouseX, &lastMouseY);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseDown = false;
                fluid->setMousePos(-1, -1);
            }
            break;
        case SDL_MOUSEMOTION:
            if (mouseDown && controlPanel->getSimulationParams().enableMouse) {
                int currentMouseX = event.motion.x;
                int currentMouseY = event.motion.y;

                fluid->setMousePos(currentMouseX, currentMouseY);

                lastMouseX = currentMouseX;
                lastMouseY = currentMouseY;
            }
            break;
        default:
            break;
        }
    }
}