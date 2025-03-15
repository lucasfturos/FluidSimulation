#include "fluid.hpp"

void Fluid::step() {
    float dt = params.dt;
    float viscosity = params.viscosity;
    float diffusion = params.diffusion;

    diffuse(1, Vx0, Vx, viscosity, dt);
    diffuse(2, Vy0, Vy, viscosity, dt);

    project(Vx0, Vy0, Vx, Vy);

    advect(1, Vx, Vx0, Vx0, Vy0, dt);
    advect(2, Vy, Vy0, Vx0, Vy0, dt);

    project(Vx, Vy, Vx0, Vy0);
    diffuse(0, s, density, diffusion, dt);
    advect(0, density, s, Vx, Vy, dt);
}

void Fluid::addDensity(int x, int y, float amount) {
    int index = IX(x, y, N);
    density[index] += amount;
}

void Fluid::addVelocity(int x, int y, float amountX, float amountY) {
    int index = IX(x, y, N);
    Vx[index] += amountX;
    Vy[index] += amountY;
}

void Fluid::addTurbulence(int x, int y, float t, float amountX, float amountY) {
    int index = IX(x, y, N);
    float noiseScale = 0.9f;
    float noiseWeight = 0.9f;
    float turbulenceStrength = 0.9f;

    float nx = static_cast<float>(x) * noiseScale;
    float ny = static_cast<float>(y) * noiseScale;

    float noiseValueX =
        perlin->noise({nx, t}) + noiseWeight * perlin->noise({2 * nx, 2 * t});
    float noiseValueY =
        perlin->noise({ny, t}) + noiseWeight * perlin->noise({2 * ny, 2 * t});

    Vx[index] += amountX * (1.0f + turbulenceStrength * noiseValueX);
    Vy[index] += amountY * (1.0f + turbulenceStrength * noiseValueY);
}

void Fluid::fadeDensity() {
    for (std::size_t i = 0; i < density.size(); ++i) {
        density[i] = std::max(density[i] - 0.02f, 0.0f);
    }
}
