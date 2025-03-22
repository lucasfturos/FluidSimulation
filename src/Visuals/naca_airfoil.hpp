#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <iostream>
#include <stdexcept>

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
    float maxCamber;
    float camber, thickness;

    Vector1f Vx, Vy;
    Vector1f Vx0, Vy0;

    float calculateCamber(float xNorm) {
        if (xNorm < maxCamber) {
            return (camber / (maxCamber * maxCamber)) *
                   (2 * maxCamber * xNorm - xNorm * xNorm);
        } else {
            return (camber / ((1 - maxCamber) * (1 - maxCamber))) *
                   ((1 - 2 * maxCamber) + 2 * maxCamber * xNorm -
                    xNorm * xNorm);
        }
    }

    float calculateThickness(float xNorm) {
        return (thickness / 0.2f) *
               (0.2969f * std::sqrt(xNorm) - 0.1260f * xNorm -
                0.3516f * xNorm * xNorm + 0.2843f * xNorm * xNorm * xNorm -
                0.1015f * xNorm * xNorm * xNorm * xNorm);
    }

  public:
    NACA_Airfoil()
        : surface(nullptr), pixels(nullptr), width(0), height(0), centerX(0),
          centerY(0), gridSize(0), airfoilWidth(100), airfoilHeight(20),
          friction(1.0f) {}

    void setSize(int size) { this->size = size; }
    void setGridSize(int size) { this->gridSize = size; }
    void setScale(int scale) { this->scale = scale; }
    void setColor(Uint32 color) { this->color = color; }
    void setFriction(float friction) { this->friction = friction; }

    void setPosition(int x, int y) {
        centerX = x;
        centerY = y;
    }

    void setDimensions(int w, int h) {
        airfoilWidth = w;
        airfoilHeight = h;
    }

    /*  Defines the NACA airfoil profile.
     * The parameters should be passed as:
     * `camber`: maximum curvature (in percentage, 0 to 9.5)
     * `maxCamber`: position of maximum camber (in percentage, 0 to 90)
     * `thickness`: maximum thickness (in percentage, 1 to 40)
     */
    void setProfile(NACA_AirfoilProfile profile) {
        this->camber = profile.camber / 100.0f;
        this->maxCamber = profile.maxCamber / 100.0f;
        this->thickness = profile.thickness / 100.0f;
    }

    void setVelocity(const Vector1f &Vx, const Vector1f &Vy) {
        this->Vx = Vx;
        this->Vy = Vy;
    }

    void setVelocity0(const Vector1f &Vx0, const Vector1f &Vy0) {
        this->Vx0 = Vx0;
        this->Vy0 = Vy0;
    }

    void setSurface(SDL_Surface *renderSurface) {
        surface = renderSurface;
        pixels = reinterpret_cast<Uint32 *>(surface->pixels);
        width = surface->w;
        height = surface->h;
    }

    Vector2f getVelocity() { return {Vx, Vy}; }
    Vector2f getVelocity0() { return {Vx0, Vy0}; }

    void collision() {}

    void draw() {
        int cx = centerX * scale;
        int cy = centerY * scale;

        for (int x = 0; x <= airfoilWidth; ++x) {
            if (x < 0 || x >= width)
                continue;

            float xNorm = static_cast<float>(x) / airfoilWidth;

            float yc = calculateCamber(xNorm);
            float yt = calculateThickness(xNorm);

            float yUpper = yc + yt;
            float yLower = yc - yt;

            int upperY = static_cast<int>(cy - airfoilHeight * yUpper);
            int lowerY = static_cast<int>(cy - airfoilHeight * yLower);

            for (int y = upperY; y <= lowerY; ++y) {
                if (y < 0 || y >= height)
                    continue;

                pixels[y * width + cx + x] = color;
            }
        }
    }
};
