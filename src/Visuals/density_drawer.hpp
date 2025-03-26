#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <functional>

class DensityDrawer {
  private:
    SDL_Surface *surface;
    Vector1f density;
    int size, scale;

    float linearInterpolate(float a, float b, float t) {
        return a + t * (b - a);
    }

    float calculateOpacity(int i, int j) {
        float topLeft = density[IX(i, j, size)];
        float topRight = density[IX(i + 1, j, size)];
        float bottomLeft = density[IX(i, j + 1, size)];
        float bottomRight = density[IX(i + 1, j + 1, size)];

        float t = 0.5f;
        float top = linearInterpolate(topLeft, topRight, t);
        float bottom = linearInterpolate(bottomLeft, bottomRight, t);
        return linearInterpolate(top, bottom, t);
    }

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

        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                int x = i * scale;
                int y = j * scale;

                float opacity = calculateOpacity(i, j);
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
