#include "fluid.hpp"

Fluid::Fluid()
    : surface(nullptr), pixels(nullptr), perlin(std::make_unique<Perlin>()),
      nacaAirfoil(std::make_unique<NACA_Airfoil>()),
      circlePhysics(std::make_unique<CirclePhysics>()),
      densityDrawer(std::make_unique<DensityDrawer>()), s(nSize, 0.0f),
      density(nSize, 0.0f), Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), mouseX(0), mouseY(0) {}

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

    nacaAirfoil->setVelocity(&Vx, &Vy);
    nacaAirfoil->setVelocity0(&Vx0, &Vy0);

    nacaAirfoil->collision();

    std::tie(Vx, Vy) = nacaAirfoil->getVelocity();
    std::tie(Vx0, Vy0) = nacaAirfoil->getVelocity0();
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

    circlePhysics->setVelocity(&Vx, &Vy);
    circlePhysics->setVelocity0(&Vx0, &Vy0);

    circlePhysics->collision();

    std::tie(Vx, Vy) = circlePhysics->getVelocity();
    std::tie(Vx0, Vy0) = circlePhysics->getVelocity0();
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

void Fluid::draw() {
    std::fill(pixels, pixels + nSize, SDL_MapRGB(surface->format, 0, 0, 0));
    step();

    int width = surface->w;
    int height = surface->h;
    int scale = params.scale;

    switch (params.object) {
    case 0:
        setupCircle(width, height, scale);
        break;
    case 1:
        setupNACA(width, height, scale);
        break;
    default:
        break;
    }

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
    case 0:
        drawCircle();
        break;
    case 1:
        drawNACA();
        break;
    default:
        break;
    }

    fadeDensity();
}
