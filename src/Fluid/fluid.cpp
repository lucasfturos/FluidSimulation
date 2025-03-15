#include "fluid.hpp"

Fluid::Fluid()
    : surface(nullptr), pixels(nullptr), perlin(std::make_shared<Perlin>()),
      circlePhysics(std::make_unique<CirclePhysics>()),
      densityDrawer(std::make_unique<DensityDrawer>()), s(nSize, 0.0f),
      density(nSize, 0.0f), Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), mouseX(0), mouseY(0) {}

void Fluid::setSurface(SDL_Surface *renderSurface) {
    surface = renderSurface;
    pixels = static_cast<Uint32 *>(renderSurface->pixels);

    circlePhysics->setSurface(surface);
    densityDrawer->setSurface(surface);
}

void Fluid::setSimulationParams(SimulationParams p) { params = p; }

void Fluid::setMousePos(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void Fluid::applyFluidInteraction(int cx, int cy, int densityValue, float vX,
                                  float vY, float turbulenceFactor) {
    addDensity(cx, cy, densityValue);
    addVelocity(cx, cy, vX, vY);
    addTurbulence(cx, cy, turbulenceFactor, vX, vY);
}

void Fluid::draw() {
    std::fill(pixels, pixels + nSize, SDL_MapRGB(surface->format, 0, 0, 0));
    step();

    int width = surface->w;
    int height = surface->h;
    int scale = params.scale;

    int radius = 10;
    int collisionX = (width * 0.1 / scale) + 30;
    int collisionY = height * 0.5 / scale;

    circlePhysics->setCollision(collisionX, collisionY);
    circlePhysics->collision();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50.0f, 350.0f);

    int cx, cy;
    static float t = 0;
    static int lastMouseX, lastMouseY;
    if (params.enableMouse) {
        cx = mouseX / scale;
        cy = mouseY / scale;

        int deltaX = mouseX - lastMouseX;
        int deltaY = mouseY - lastMouseY;

        float vX = deltaX * 0.2f;
        float vY = deltaY * 0.2f;

        applyFluidInteraction(cx, cy, 350.0f, vX, vY, 0.01f);

        lastMouseX = mouseX;
        lastMouseY = mouseY;
    } else {
        cx = static_cast<int>(width * 0.1 / scale);
        cy = static_cast<int>(height * 0.5 / scale);
        for (int i = 0; i < 6; ++i) {
            float angle = 2 * M_PI;
            float vX = std::cos(angle) * 0.1f;
            float vY = std::sin(angle) * 0.1f;

            applyFluidInteraction(cx, cy, dis(gen), vX, vY, t);
            t += 0.01f;
        }
    }

    densityDrawer->setSize(N);
    densityDrawer->setScale(params.scale);
    densityDrawer->setDensityData(density);
    densityDrawer->draw();

    Uint32 fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    circlePhysics->setPosition(collisionX * scale, collisionY * scale);
    circlePhysics->setRadius(radius);
    circlePhysics->setColor(fillColor);
    circlePhysics->draw();

    fadeDensity();
}
