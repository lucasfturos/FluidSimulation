#pragma once

#include "../FluidSimulation/fluid_simulation.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <string>

class Render {
  protected:
    const std::string title = "Fluid Simulation";
    const int screenWidth = 1080;
    const int screenHeight = 720;
    const int fps = 60;
    const int frameDelay = 1000 / fps;

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;

    bool quit;
    std::shared_ptr<FluidSimulation> fluidSimulation;

  private:
    Uint32 frameStart;
    int frameTime;

    void setupWindow();
    void destroyWindow();

    void handleEvents();
    void draw();

  public:
    Render();
    ~Render();
    void run();
};