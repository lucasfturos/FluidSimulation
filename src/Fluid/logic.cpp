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

void Fluid::applyFluidInteraction(int cx, int cy, int densityValue, float vX,
                                  float vY, float turbulenceFactor) {
    addDensity(cx, cy, densityValue);
    addVelocity(cx, cy, vX, vY);
    addTurbulence(cx, cy, turbulenceFactor, vX, vY);
}

void Fluid::applyMouseInteraction(int &lastMouseX, int &lastMouseY, int scale) {

    int cx = mouseX / scale;
    int cy = mouseY / scale;

    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;

    float vX = deltaX * 0.2f;
    float vY = deltaY * 0.2f;

    applyFluidInteraction(cx, cy, 350.0f, vX, vY, 0.01f);

    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void Fluid::applyRandomInteraction(int width, int height, int scale, float &t) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50.0f, 350.0f);

    int cx = static_cast<int>(width * 0.1 / scale);
    int cy = static_cast<int>(height * 0.5 / scale);

    for (int i = 0; i < 6; ++i) {
        float angle = 2 * M_PI;
        float vX = std::cos(angle) * 0.1f;
        float vY = std::sin(angle) * 0.1f;

        applyFluidInteraction(cx, cy, dis(gen), vX, vY, t);
        t += 0.01f;
    }
}

void Fluid::fadeDensity() {
    for (std::size_t i = 0; i < density.size(); ++i) {
        density[i] = std::max(density[i] - 0.02f, 0.0f);
    }
}
