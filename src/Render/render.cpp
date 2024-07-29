#include "render.hpp"

Render::Render()
    : window(nullptr), renderer(nullptr), texture(nullptr), surface(nullptr),
      quit(false), fluidSimulation(std::make_shared<FluidSimulation>()) {
    setupWindow();
    fluidSimulation->setSurface(surface);
    fluidSimulation->setFilename("../simulation_params.json");
}

Render::~Render() { destroyWindow(); }

void Render::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
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
        default:
            break;
        }
    }
}

void Render::draw() {
    if (SDL_LockSurface(surface) == 0) {
        SDL_memset(surface->pixels, 0, surface->h * surface->pitch);

        fluidSimulation->draw();

        SDL_UnlockSurface(surface);
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }
}

void Render::run() {
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
    SDL_Quit();
}