#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <functional>

class DensityDrawer {
  private:
    SDL_Surface *surface;
    std::vector<float> density;
    int size, scale;

  public:
    DensityDrawer() : surface(nullptr) {}

    void setSize(int size) { this->size = size; }
    void setScale(int scaleFactor) { this->scale = scaleFactor; }
    void setSurface(SDL_Surface *surface) { this->surface = surface; }

    void setDensityData(std::vector<float> &densityField) {
        this->density = densityField;
    }

    void draw() {
        int width = surface->w;
        int height = surface->h;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int x = i * scale;
                int y = j * scale;
                Uint8 opacity = density[IX(i, j, size)];
                Uint32 color =
                    getColorByValue(std::fmod((opacity + 50), 255.0f),
                                    200 / 255.0f, opacity / 255.0f);
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    SDL_Rect rect = {x, y, scale, scale};
                    SDL_FillRect(surface, &rect, color);
                }
            }
        }
    }
};
