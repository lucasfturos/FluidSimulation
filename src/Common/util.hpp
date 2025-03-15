#pragma once

#include <SDL2/SDL_stdinc.h>
#include <algorithm>
#include <cmath>
#include <vector>

template <typename T = Uint8> struct Color { T r, g, b, a = 0; };

using Colori = Color<>;
using Colorf = Color<float>;

inline Uint32 HSV2RGB(float h, float s, float v) {
    Colorf color;
    int i = static_cast<int>(std::floor(h * 6));
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
    case 0:
        color = {v, t, p};
        break;
    case 1:
        color = {q, v, p};
        break;
    case 2:
        color = {p, v, t};
        break;
    case 3:
        color = {p, q, v};
        break;
    case 4:
        color = {t, p, v};
        break;
    case 5:
        color = {v, p, q};
        break;
    default:
        color = {0.0f, 0.0f, 0.0f};
        break;
    }

    return (static_cast<Uint32>(255) << 24) |
           (static_cast<Uint32>(color.r * 255) << 16) |
           (static_cast<Uint32>(color.g * 255) << 8) |
           static_cast<Uint32>(color.b * 255);
}

inline Uint32 getColorByValue(int value, float saturation = 0.8f,
                              float brightness = 0.8f) {
    float hue = static_cast<float>(value) / 255.0f;
    return HSV2RGB(hue, saturation, brightness);
}

inline float toRad(float angle) { return angle * M_PI / 180.0f; }

/**
 * Function of indexing the 1D array
 * - x : int
 * - y : int
 * Returns the index for a given (x, y) pair
 */
inline int IX(int x, int y, int size) {
    x = std::clamp(x, 0, size - 1);
    y = std::clamp(y, 0, size - 1);
    return x + y * size;
}

struct SimulationParams {
    int iter;
    int scale;
    float diffusion;
    float viscosity;
    float dt;
    bool enableMouse;
};