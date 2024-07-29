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

void FluidSimulation::drawLine(int x1, int y1, int x2, int y2, Uint32 color) {
    int width = surface->w;
    int height = surface->h;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
            pixels[y1 * width + x1] = color;
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void FluidSimulation::drawDensity() {
    int width = surface->w;
    int height = surface->h;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x = i * SCALE;
            int y = j * SCALE;
            Uint8 opacity = density[IX(i, j)];
            Uint32 color = getColorByValue(std::fmod((opacity + 50), 255.0f),
                                           200 / 255.0f, opacity / 255.0f);
            if (x >= 0 && x < width && y >= 0 && y < height) {
                SDL_Rect rect = {x, y, SCALE, SCALE};
                SDL_FillRect(surface, &rect, color);
            }
        }
    }
}

void FluidSimulation::drawVelocity() {
    Uint32 fillColor = SDL_MapRGBA(surface->format, 0, 0, 0, 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x = i * SCALE;
            int y = j * SCALE;

            float vx = Vx[IX(i, j)];
            float vy = Vy[IX(i, j)];

            int endX = static_cast<int>(x + vx * SCALE);
            int endY = static_cast<int>(y + vy * SCALE);
            drawLine(x, y, endX, endY, fillColor);
        }
    }
}

void FluidSimulation::draw() {
    static float t = 0;
    std::fill(pixels, pixels + nSize, SDL_MapRGB(surface->format, 0, 0, 0));
    step();

    int width = surface->w;
    int height = surface->h;

    int cx = static_cast<int>(width * 0.2 / SCALE);
    int cy = static_cast<int>(height * 0.5 / SCALE);

    int radius = 10;
    int collisionX = cx + 10;
    int collisionY = cy;

    updateCircleCollision(collisionX, collisionY, radius);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50.0f, 150.0f);

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            addDensity(cx + i, cy + j, dis(gen));
        }
    }

    for (int i = 0; i < 2; ++i) {
        float angle = perlin->turb({t * 0.4f, 0.0f}, 3) * 2.0f * M_PI;
        float vX = std::cos(angle) * 0.1f;
        float vY = std::sin(angle) * 0.1f;
        addVelocity(cx, cy, vX, vY);
        addTurbulence(cx, cy, t, vX, vY);
        t += 0.01f;
    }

    drawDensity();
    drawVelocity();

    Uint32 fillColor = SDL_MapRGB(surface->format, 255, 0, 0);
    drawCircle(collisionX * SCALE, collisionY * SCALE, radius, fillColor);

    fadeDensity();
}
