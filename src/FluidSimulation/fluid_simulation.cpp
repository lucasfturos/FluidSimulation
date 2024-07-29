#include "fluid_simulation.hpp"

FluidSimulation::FluidSimulation()
    : surface(nullptr), pixels(nullptr), perlin(std::make_shared<Perlin>()),
      s(nSize, 0.0f), density(nSize, 0.0f), Vx(nSize, 0.0f), Vy(nSize, 0.0f),
      Vx0(nSize, 0.0f), Vy0(nSize, 0.0f) {}

void FluidSimulation::setSurface(SDL_Surface *renderSurface) {
    surface = renderSurface;
    pixels = static_cast<Uint32 *>(renderSurface->pixels);
}

void FluidSimulation::setFilename(const std::string &filepath) {
    filename = filepath;
    loadParams();
}

void FluidSimulation::loadParams() {
    std::ifstream file(filename, std::ifstream::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    Json::Value root;
    file >> root;
    params = {
        .diffusion = root.get("diffusion", 1.0e-5).asFloat(),
        .viscosity = root.get("viscosity", 1.0e-6).asFloat(),
        .dt = root.get("dt", 0.01).asFloat(),
    };
}
