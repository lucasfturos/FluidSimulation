#include "fluid.hpp"

Fluid::Fluid()
    : surface(nullptr), pixels(nullptr), perlin(std::make_unique<Perlin>()),
      nacaAirfoil(std::make_unique<NACA_Airfoil>()),
      circlePhysics(std::make_unique<CirclePhysics>()),
      densityDrawer(std::make_unique<DensityDrawer>()), s(nSize, 0.0f),
      density(nSize, 0.0f), Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), width(0), height(0), mouseX(0), mouseY(0), scale(0) {}

void Fluid::setup() {
    width = surface->w;
    height = surface->h;
    scale = params.scale;

    setupCircle(width, height, scale);
    setupNACA(width, height, scale);
}

void Fluid::setSurface(SDL_Surface *renderSurface) {
    surface = renderSurface;
    pixels = static_cast<Uint32 *>(renderSurface->pixels);

    nacaAirfoil->setSurface(surface);
    circlePhysics->setSurface(surface);
    densityDrawer->setSurface(surface);
}

void Fluid::setSimulationParams(SimulationParams p) { params = p; }

void Fluid::setMousePos(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void Fluid::setupNACA(int width, int height, int scale) {
    nacaAirfoil->setDimensions(500, 200);
    nacaAirfoil->setProfile(0.02f, 0.12f);

    nacaAirfoil->setVelocity(&Vx, &Vy);
    nacaAirfoil->setVelocity0(&Vx0, &Vy0);

    int centerX = (width * 0.1 / scale) + 30;
    int centerY = (height * 0.5 / scale) + 1;

    nacaAirfoil->setPosition(centerX, centerY);
    nacaAirfoil->setSize(N);
    nacaAirfoil->setScale(scale);
}

void Fluid::drawNACA() {
    auto fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    nacaAirfoil->setColor(fillColor);
    nacaAirfoil->draw();
}

void Fluid::setupCircle(int width, int height, int scale) {
    circlePhysics->setSize(N);
    circlePhysics->setScale(scale);

    int radius = 10;
    circlePhysics->setRadius(radius);

    int positionX = (width * 0.1 / scale) + 30;
    int positionY = (height * 0.5 / scale) + 1;
    circlePhysics->setPosition(positionX, positionY);
}

void Fluid::drawCircle() {
    auto fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    circlePhysics->setColor(fillColor);
    circlePhysics->draw();
}

void Fluid::drawDensity(int scale) {
    densityDrawer->setSize(N);
    densityDrawer->setScale(scale);
    densityDrawer->setDensityData(density);
    densityDrawer->draw();
}

void Fluid::draw() {
    std::fill(pixels, pixels + nSize, SDL_MapRGB(surface->format, 0, 0, 0));
    step();

    static float t = 0;
    static int lastMouseX, lastMouseY;
    if (params.enableMouse) {
        applyMouseInteraction(lastMouseX, lastMouseY, scale);
    }
    if (!params.enableMouse) {
        applyRandomInteraction(width, height, scale, t);
    }

    drawDensity(scale);

    switch (params.object) {
    case 0: {
        circlePhysics->setVelocity(&Vx, &Vy);
        circlePhysics->setVelocity0(&Vx0, &Vy0);

        circlePhysics->collision();

        std::tie(Vx, Vy) = circlePhysics->getVelocity();
        std::tie(Vx0, Vy0) = circlePhysics->getVelocity0();

        drawCircle();
    } break;
    case 1: {
        nacaAirfoil->setVelocity(&Vx, &Vy);
        nacaAirfoil->setVelocity0(&Vx0, &Vy0);

        nacaAirfoil->collision();

        std::tie(Vx, Vy) = nacaAirfoil->getVelocity();
        std::tie(Vx0, Vy0) = nacaAirfoil->getVelocity0();

        drawNACA();
    } break;
    default:
        break;
    }

    fadeDensity();
}
