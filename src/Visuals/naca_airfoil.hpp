#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>

class NACA_Airfoil {
  private:
    SDL_Surface *surface;

    Uint32 *pixels;
    Uint32 color;

    int width, height;
    int centerX, centerY, gridSize;
    int airfoilWidth, airfoilHeight;
    int size, scale;

    float friction;
    float camber, thickness;

    Vector1f Vx, Vy;
    Vector1f Vx0, Vy0;

  public:
    NACA_Airfoil()
        : surface(nullptr), pixels(nullptr), width(0), height(0), centerX(0),
          centerY(0), gridSize(0), airfoilWidth(100), airfoilHeight(20),
          friction(1.0f), camber(0.02f), thickness(0.12f) {}

    void setSize(int size) { this->size = size; }
    void setGridSize(int size) { gridSize = size; }
    void setScale(int scale) { this->scale = scale; }
    void setColor(Uint32 color) { this->color = color; }
    void setFriction(float frictionValue) { friction = frictionValue; }

    void setPosition(int x, int y) {
        centerX = x;
        centerY = y;
    }

    void setDimensions(int w, int h) {
        airfoilWidth = w;
        airfoilHeight = h;
    }

    void setProfile(float camberValue, float thicknessValue) {
        camber = camberValue;
        thickness = thicknessValue;
    }

    void setVelocity(Vector1f *Vx, Vector1f *Vy) {
        this->Vx = *Vx;
        this->Vy = *Vy;
    }

    void setVelocity0(Vector1f *Vx0, Vector1f *Vy0) {
        this->Vx0 = *Vx0;
        this->Vy0 = *Vy0;
    }

    void setSurface(SDL_Surface *renderSurface) {
        surface = renderSurface;
        pixels = static_cast<Uint32 *>(surface->pixels);
        width = surface->w;
        height = surface->h;
    }

    const Vector2f getVelocity() const { return {Vx, Vy}; }
    const Vector2f getVelocity0() const { return {Vx0, Vy0}; }

    void collision() {}

    void draw() {
        int cx = centerX * scale;
        int cy = centerY * scale;

        for (int x = 0; x <= airfoilWidth; ++x) {
            if (x < 0 || x >= width)
                continue;

            float xNorm = static_cast<float>(x) / airfoilWidth;
            float yt =
                thickness * 5.0f *
                (0.2969f * std::sqrt(xNorm) - 0.1260f * xNorm -
                 0.3516f * (xNorm * xNorm) + 0.2843f * (xNorm * xNorm * xNorm) -
                 0.1015f * (xNorm * xNorm * xNorm * xNorm));
            float yc = camber * xNorm;

            int upperY = static_cast<int>(cy - airfoilHeight * (yc + yt));
            int lowerY = static_cast<int>(cy - airfoilHeight * (yc - yt));

            for (int y = upperY; y <= lowerY; ++y) {
                if (y < 0 || y >= height)
                    continue;

                pixels[y * width + cx + x] = color;
            }
        }
    }
};
