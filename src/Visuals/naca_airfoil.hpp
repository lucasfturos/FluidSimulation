#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>

class NACA_Airfoil {
  private:
    SDL_Surface *surface;

    Uint32 *pixels;
    Uint32 color;

    int width, height;
    int positionX, positionY, gridSize;
    int airfoilWidth, airfoilHeight;
    int size, scale;

    float friction;
    float maxCamber;
    float camber, thickness;

    Vector1f Vx, Vy;
    Vector1f Vx0, Vy0;

    float calculateCamber(float x) {
        if (x < maxCamber) {
            return (camber / (maxCamber * maxCamber)) *
                   (2 * maxCamber * x - x * x);
        } else {
            return (camber / ((1 - maxCamber) * (1 - maxCamber))) *
                   ((1 - 2 * maxCamber) + 2 * maxCamber * x - x * x);
        }
    }

    float calculateThickness(float x) {
        return (thickness / 0.2f) *
               (0.2969f * std::sqrt(x) - 0.1260f * x - 0.3516f * x * x +
                0.2843f * x * x * x - 0.1015f * x * x * x * x);
    }

  public:
    NACA_Airfoil()
        : surface(nullptr), pixels(nullptr), width(0), height(0), positionX(0),
          positionY(0), gridSize(0), airfoilWidth(100), airfoilHeight(20),
          friction(1.0f) {}

    void setSize(int size) { this->size = size; }
    void setScale(int scale) { this->scale = scale; }
    void setColor(Uint32 color) { this->color = color; }
    void setGridSize(int size) { this->gridSize = size; }
    void setFriction(float friction) { this->friction = friction; }

    void setPosition(int x, int y) {
        positionX = x;
        positionY = y;
    }

    void setDimensions(int w, int h) {
        airfoilWidth = w;
        airfoilHeight = h;
    }

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

    void collision() {
        for (int x = 0; x <= airfoilWidth; ++x) {
            if (x < 0 || x >= width)
                continue;

            float xNorm = static_cast<float>(x) / airfoilWidth;

            float yc = calculateCamber(xNorm);
            float yt = calculateThickness(xNorm);

            float yUpper = yc + yt;
            float yLower = yc - yt;

            int upperY = static_cast<int>(positionY - airfoilHeight * yUpper);
            int lowerY = static_cast<int>(positionY - airfoilHeight * yLower);

            for (int y = upperY; y <= lowerY; ++y) {
                if (y < 0 || y >= height)
                    continue;

                int index = IX(x, y, gridSize);

                int dx = x - positionX;
                int dy = y - positionY;

                float length = std::sqrt(dx * dx + dy * dy);
                if (length > 0.0f) {
                    float nx = dx / length;
                    float ny = dy / length;

                    float dot = Vx[index] * nx + Vy[index] * ny;
                    float tx = Vx[index] - dot * nx;
                    float ty = Vy[index] - dot * ny;

                    tx *= friction;
                    ty *= friction;

                    Vx[index] = Vx0[index] = tx;
                    Vy[index] = Vy0[index] = ty;
                } else {
                    Vx[index] = Vy[index] = 0.0f;
                    Vx0[index] = Vy0[index] = 0.0f;
                }
            }
        }
    }

    void draw() {
        int cx = positionX * scale;
        int cy = positionY * scale;

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
