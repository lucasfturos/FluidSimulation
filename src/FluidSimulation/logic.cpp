#include "fluid_simulation.hpp"

void FluidSimulation::step() {
    diffuse(1, Vx0, Vx, params.viscosity, params.dt);
    diffuse(2, Vy0, Vy, params.viscosity, params.dt);

    project(Vx0, Vy0, Vx, Vy);

    advect(1, Vx, Vx0, Vx0, Vy0, params.dt);
    advect(2, Vy, Vy0, Vx0, Vy0, params.dt);

    project(Vx, Vy, Vx0, Vy0);
    diffuse(0, s, density, params.diffusion, params.dt);
    advect(0, density, s, Vx, Vy, params.dt);
}

void FluidSimulation::addDensity(int x, int y, float amount) {
    int index = IX(x, y);
    density[index] += amount;
}

void FluidSimulation::addVelocity(int x, int y, float amountX, float amountY) {
    int index = IX(x, y);
    Vx[index] += amountX;
    Vy[index] += amountY;
}

void FluidSimulation::addTurbulence(int x, int y, float t, float amountX,
                                    float amountY) {
    int index = IX(x, y);
    Vx[index] +=
        amountX * (1.0f + 0.1f * perlin->noise({static_cast<float>(x), t}));
    Vy[index] +=
        amountY * (1.0f + 0.1f * perlin->noise({static_cast<float>(y), t}));
}

void FluidSimulation::fadeDensity() {
    for (std::size_t i = 0; i < density.size(); ++i) {
        density[i] = std::max(density[i] - 0.02f, 0.0f);
    }
}

void FluidSimulation::applyCircleCollision(int cx, int cy, int radius) {
    for (int j = cy - radius; j <= cy + radius; ++j) {
        for (int i = cx - radius; i <= cx + radius; ++i) {
            if (i >= 0 && i < N && j >= 0 && j < N) {
                int dx = i - cx;
                int dy = j - cy;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    Vx[IX(i, j)] = 0.0f;
                    Vy[IX(i, j)] = 0.0f;
                    Vx0[IX(i, j)] = 0.0f;
                    Vy0[IX(i, j)] = 0.0f;
                }
            }
        }
    }
}
