#include "../Common/util.hpp"
#include "fluid_simulation.hpp"

void FluidSimulation::drawCircle(int cx, int cy, int radius, Uint32 color) {
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

void FluidSimulation::drawDensity() {
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

void FluidSimulation::draw() {
    static float t = 0;
    std::fill(pixels, pixels + nSize, SDL_MapRGB(surface->format, 0, 0, 0));
    step();

    int width = surface->w;
    int height = surface->h;
    int scale = params.scale;

    int cx = static_cast<int>(width * 0.1 / scale);
    int cy = static_cast<int>(height * 0.5 / scale);

    int radius = 10;
    int collisionX = cx + 30;
    int collisionY = cy;
    updateCircleCollision(collisionX, collisionY, radius);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50.0f, 250.0f);

    for (int i = 0; i < 4; ++i) {
        addDensity(cx, cy, dis(gen));
    }

    for (int i = 0; i < 4; ++i) {
        float angle = 2 * M_PI;
        float vX = std::cos(angle) * 0.1f;
        float vY = std::sin(angle) * 0.1f;
        addVelocity(cx, cy, vX, vY);
        addTurbulence(cx, cy, t, vX, vY);
        t += 0.01f;
    }

    drawDensity();

    Uint32 fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    drawCircle(collisionX * scale, collisionY * scale, radius, fillColor);

    fadeDensity();
}
