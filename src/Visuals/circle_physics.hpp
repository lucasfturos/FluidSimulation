#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <cmath>
#include <vector>

class CirclePhysics {
  private:
    SDL_Surface *surface;

    Uint32 *pixels;
    Uint32 color;

    int circleX, circleY;
    int radius, gridSize, scale;

    float friction;

    Vector1f Vx, Vy;
    Vector1f Vx0, Vy0;

  public:
    CirclePhysics() : surface(nullptr), pixels(nullptr), friction(1.0f) {}

    void setSize(int size) { this->gridSize = size; }
    void setScale(int scale) { this->scale = scale; }
    void setColor(Uint32 color) { this->color = color; }
    void setRadius(int radius) { this->radius = radius; }
    void setFriction(float friction) { this->friction = friction; }

    void setPosition(int x, int y) {
        this->circleX = x;
        this->circleY = y;
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
        pixels = reinterpret_cast<Uint32 *>(renderSurface->pixels);
    }

    Vector2f getVelocity() { return {Vx, Vy}; }
    Vector2f getVelocity0() { return {Vx0, Vy0}; }

    void collision() {
        int startX = std::max(0, circleX - radius);
        int endX = std::min(gridSize - 1, circleX + radius);
        int startY = std::max(0, circleY - radius);
        int endY = std::min(gridSize - 1, circleY + radius);

        for (int j = startY; j <= endY; ++j) {
            for (int i = startX; i <= endX; ++i) {
                int dx = i - circleX;
                int dy = j - circleY;
                if ((dx * dx + dy * dy) <= ((radius * radius) / scale)) {
                    int index = IX(i, j, gridSize);
                    float length = std::sqrt(dx * dx + dy * dy);
                    if (length > 0.0f) {
                        float nx = dx / length;
                        float ny = dy / length;

                        float dotProduct = Vx[index] * nx + Vy[index] * ny;
                        float tx = Vx[index] - dotProduct * nx;
                        float ty = Vy[index] - dotProduct * ny;

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
    }

    void draw() {
        int cx = circleX * scale;
        int cy = circleY * scale;
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