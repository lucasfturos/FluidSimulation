#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <functional>

class DensityDrawer {
  private:
    SDL_Surface *surface;
    Vector1f density;
    int size, scale;

  public:
    DensityDrawer() : surface(nullptr) {}

    void setSize(int size) { this->size = size; }
    void setScale(int scaleFactor) { this->scale = scaleFactor; }
    void setSurface(SDL_Surface *surface) { this->surface = surface; }

    void setDensityData(Vector1f &densityField) {
        this->density = densityField;
    }

    void draw() {
        int width = surface->w;
        int height = surface->h;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int x = i * scale;
                int y = j * scale;
                auto opacity = static_cast<Uint8>(density[IX(i, j, size)]);
                auto color = getColorByValue(std::fmod((opacity + 50), 255.0f),
                                             200 / 255.0f, opacity / 255.0f);
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    SDL_Rect rect{x, y, scale, scale};
                    SDL_FillRect(surface, &rect, color);
                }
            }
        }
    }
};
