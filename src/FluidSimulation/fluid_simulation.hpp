#pragma once

#include "../Common/perlin.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <memory>
#include <vector>

class FluidSimulation {
  protected:
    struct SimulationParameters {
        int iter;
        int scale;
        float diffusion;
        float viscosity;
        float dt;
    };

    const int N = 270;
    const int nSize = N * N;

  private:
    SDL_Surface *surface;
    Uint32 *pixels;
    std::shared_ptr<Perlin> perlin;

    std::vector<float> s;
    std::vector<float> density;
    std::vector<float> Vx, Vy;
    std::vector<float> Vx0, Vy0;

  private:
    std::string filename;
    SimulationParameters params;

    void loadParams();

    // Logic
    void step();
    void addDensity(int, int, float);
    void addVelocity(int, int, float, float);
    void addTurbulence(int, int, float, float, float);
    void updateCircleCollision(int, int, int, float = 1.0f);
    void fadeDensity();

    // Draw
    void drawCircle(int, int, int, Uint32);
    void drawDensity();

    // Util
    int IX(int, int);
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
    FluidSimulation();

    void setSurface(SDL_Surface *);
    void setFilename(const std::string &);
    void draw();
};