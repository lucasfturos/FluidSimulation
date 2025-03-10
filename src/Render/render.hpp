#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Fluid/fluid.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <functional>

static std::function<void()> loop;
static inline void mainLoop() { loop(); }
#endif

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
    std::shared_ptr<Fluid> fluid;
    std::shared_ptr<ControlPanel> controlPanel;

  private:
    Uint32 frameStart;
    int frameTime;

    float scaleFactorX() const;
    float scaleFactorY() const;

    // Draw
    void draw();

    // Event
    void handleEvents();

    // Setup
    void setup();
    void setupImGui();
    void setupWindow();

  public:
    Render();
    ~Render();
    void run();
};