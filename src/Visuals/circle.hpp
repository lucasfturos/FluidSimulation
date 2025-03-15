#pragma once

#include <SDL_surface.h>
#include <cmath>
#include <vector>

class CircleDrawer {
  private:
    SDL_Surface *surface;
    Uint32 *pixels;
    int cx, cy;
    int radius;
    Uint32 color;

  public:
    CircleDrawer() : surface(nullptr), pixels(nullptr) {}

    void setX(int x) { this->cx = x; }
    void setY(int y) { this->cy = y; }
    void setColor(Uint32 color) { this->color = color; }
    void setRadius(int radius) { this->radius = radius; }
    void setSurface(SDL_Surface *renderSurface) {
        surface = renderSurface;
        pixels = static_cast<Uint32 *>(renderSurface->pixels);
    }

    void draw() {
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
};