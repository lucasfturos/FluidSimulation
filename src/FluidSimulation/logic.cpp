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

void FluidSimulation::fadeDensity() {
    for (std::size_t i = 0; i < density.size(); ++i) {
        density[i] = std::max(density[i] - 0.02f, 0.0f);
    }
}

void FluidSimulation::updateCircleCollision(int cx, int cy, int radius,
                                            float friction) {
    int startX = std::max(0, cx - radius);
    int endX = std::min(N - 1, cx + radius);
    int startY = std::max(0, cy - radius);
    int endY = std::min(N - 1, cy + radius);

    for (int j = startY; j <= endY; ++j) {
        for (int i = startX; i <= endX; ++i) {
            int dx = i - cx;
            int dy = j - cy;
            if ((dx * dx + dy * dy) <= ((radius * radius) / SCALE)) {
                int index = IX(i, j);
                float length = std::sqrt(dx * dx + dy * dy);
                if (length > 0) {
                    float nx = dx / length;
                    float ny = dy / length;

                    float dotProduct = Vx[index] * nx + Vy[index] * ny;
                    float tx = Vx[index] - dotProduct * nx;
                    float ty = Vy[index] - dotProduct * ny;

                    tx *= friction;
                    ty *= friction;

                    Vx[index] = tx;
                    Vy[index] = ty;

                    Vx0[index] = tx;
                    Vy0[index] = ty;
                } else {
                    Vx[index] = 0.0f;
                    Vy[index] = 0.0f;
                    Vx0[index] = 0.0f;
                    Vy0[index] = 0.0f;
                }
            }
        }
    }
}
