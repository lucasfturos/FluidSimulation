#pragma once

#include "Common/perlin.hpp"
#include "Common/util.hpp"

#include "Visuals/circle_physics.hpp"
#include "Visuals/density.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

class Fluid {
  protected:
    const int N = 270;
    const int nSize = N * N;

  private:
    SDL_Surface *surface;
    Uint32 *pixels;
    std::shared_ptr<Perlin> perlin;
    std::unique_ptr<CirclePhysics> circlePhysics;
    std::unique_ptr<DensityDrawer> densityDrawer;

    std::vector<float> s;
    std::vector<float> density;
    std::vector<float> Vx, Vy;
    std::vector<float> Vx0, Vy0;
    int mouseX, mouseY;

  private:
    SimulationParams params;

    void applyFluidInteraction(int, int, int, float, float, float);

    // Logic
    void step();
    void addDensity(int, int, float);
    void addVelocity(int, int, float, float);
    void addTurbulence(int, int, float, float, float);
    void fadeDensity();

    // Util
    void setBND(int, std::vector<float> &);
    void project(std::vector<float> &, std::vector<float> &,
                 std::vector<float> &, std::vector<float> &);
    void advect(int, std::vector<float> &, const std::vector<float> &,
                const std::vector<float> &, const std::vector<float> &, float);
    void linSolve(int, std::vector<float> &, const std::vector<float> &, float,
                  float);
    void diffuse(int, std::vector<float> &x, const std::vector<float> &, float,
                 float);

  public:
    Fluid();

    void setSurface(SDL_Surface *);
    void setMousePos(int, int);
    void setSimulationParams(SimulationParams);
    void draw();
};