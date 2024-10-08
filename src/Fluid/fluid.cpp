#include "fluid.hpp"

Fluid::Fluid()
    : surface(nullptr), pixels(nullptr), perlin(std::make_shared<Perlin>()),
      s(nSize, 0.0f), density(nSize, 0.0f), Vx(nSize, 0.0f), Vy(nSize, 0.0f),
      Vx0(nSize, 0.0f), Vy0(nSize, 0.0f), mouseX(0), mouseY(0) {}

void Fluid::setSurface(SDL_Surface *renderSurface) {
    surface = renderSurface;
    pixels = static_cast<Uint32 *>(renderSurface->pixels);
}

void Fluid::setSimulationParams(SimulationParams p) { params = p; }

void Fluid::setMousePos(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void Fluid::drawCircle(int cx, int cy, int radius, Uint32 color) {
    int width = surface->w;
    int height = surface->h;

    for (int y = cy - radius; y <= cy + radius; ++y) {
        if (y < 0 || y >= height)
            continue;
        for (int x = cx - radius; x <= cx + radius; ++x) {
            if (x < 0 || x >= width)
                continue;
            int dx = x - cx;
            int dy = y - cy;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                pixels[y * width + x] = color;
            }
        }
    }
}

void Fluid::drawDensity() {
    int width = surface->w;
    int height = surface->h;
    int scale = params.scale;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x = i * scale;
            int y = j * scale;
            Uint8 opacity = density[IX(i, j)];
            Uint32 color = getColorByValue(std::fmod((opacity + 50), 255.0f),
                                           200 / 255.0f, opacity / 255.0f);
            if (x >= 0 && x < width && y >= 0 && y < height) {
                SDL_Rect rect = {x, y, scale, scale};
                SDL_FillRect(surface, &rect, color);
            }
        }
    }
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
    updateCircleCollision(collisionX, collisionY, radius);

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

    drawDensity();

    Uint32 fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    drawCircle(collisionX * scale, collisionY * scale, radius, fillColor);

    fadeDensity();
}
