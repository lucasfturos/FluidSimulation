#pragma once

#include "Common/util.hpp"

#include <SDL_surface.h>
#include <cmath>
#include <vector>

class CirclePhysics {
  protected:
    const float eps = 1e-6f;

  private:
    SDL_Surface *surface;

    Uint32 *pixels;
    Uint32 color;

    int circleX, circleY;
    int collisionX, collisionY;
    int radius, size, scale;

    float friction;

    std::vector<float> Vx, Vy;
    std::vector<float> Vx0, Vy0;

  public:
    CirclePhysics() : surface(nullptr), pixels(nullptr), friction(1.0f) {}

    void setSize(int size) { this->size = size; }
    void setScale(int scale) { this->scale = scale; }
    void setColor(Uint32 color) { this->color = color; }
    void setRadius(int radius) { this->radius = radius; }
    void setFriction(float friction) { this->friction = friction; }

    void setPosition(int x, int y) {
        this->circleX = x * this->scale;
        this->circleY = y * this->scale;
    }
    void setPosCollision(int x, int y) {
        this->collisionX = x;
        this->collisionY = y;
    }
    void setVelocity0(std::vector<float> *Vx0, std::vector<float> *Vy0) {
        this->Vx0 = *Vx0;
        this->Vy0 = *Vy0;
    }
    void setVelocity(std::vector<float> *Vx, std::vector<float> *Vy) {
        this->Vx = *Vx;
        this->Vy = *Vy;
    }

    void setSurface(SDL_Surface *renderSurface) {
        surface = renderSurface;
        pixels = static_cast<Uint32 *>(renderSurface->pixels);
    }

    std::pair<std::vector<float> &, std::vector<float> &> getVelocity() {
        return {Vx, Vy};
    }

    std::pair<std::vector<float> &, std::vector<float> &> getVelocity0() {
        return {Vx0, Vy0};
    }

    void collision() {
        int startX = std::max(0, collisionX - radius);
        int endX = std::min(size - 1, collisionX + radius);
        int startY = std::max(0, collisionY - radius);
        int endY = std::min(size - 1, collisionY + radius);

        for (int j = startY; j <= endY; ++j) {
            for (int i = startX; i <= endX; ++i) {
                int dx = i - collisionX;
                int dy = j - collisionY;
                if ((dx * dx + dy * dy) <= ((radius * radius) / scale)) {
                    int index = IX(i, j, size);
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
        int width = surface->w;
        int height = surface->h;

        for (int y = circleY - radius; y <= circleY + radius; ++y) {
            if (y < 0 || y >= height)
                continue;
            for (int x = circleX - radius; x <= circleX + radius; ++x) {
                if (x < 0 || x >= width)
                    continue;
                int dx = x - circleX;
                int dy = y - circleY;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    pixels[y * width + x] = color;
                }
            }
        }
    }
};