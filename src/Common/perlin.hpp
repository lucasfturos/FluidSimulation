#pragma once

#include <array>
#include <cmath>
#include <memory>
#include <random>

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
    return static_cast<int>(randomDouble(min, max + 1));
}

using point2 = std::array<double, 2>;

inline double dot(const point2 &u, const point2 &v) {
    return u[0] * v[0] + u[1] * v[1];
}

inline point2 unitVector(const point2 &v) {
    double length = std::sqrt(dot(v, v));
    return {v[0] / length, v[1] / length};
}

class Perlin {
  public:
    Perlin() {
        ranvec = std::make_unique<point2[]>(point_count);
        for (int i = 0; i < point_count; ++i) {
            ranvec[i] = unitVector({randomDouble(-1, 1), randomDouble(-1, 1)});
        }

        perm_x = perlinGeneratePerm();
        perm_y = perlinGeneratePerm();
    }

    double noise(const point2 &p) const {
        auto u = p[0] - std::floor(p[0]);
        auto v = p[1] - std::floor(p[1]);
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);

        auto i = static_cast<int>(std::floor(p[0]));
        auto j = static_cast<int>(std::floor(p[1]));
        std::array<std::array<point2, 2>, 2> c;

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                c[di][dj] =
                    ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255]];
            }
        }
        return perlinInterp(c, u, v);
    }

    double turb(const point2 &p, int depth = 7) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p[0] *= 2;
            temp_p[1] *= 2;
        }

        return std::fabs(accum);
    }

  private:
    static const int point_count = 256;
    std::unique_ptr<point2[]> ranvec;
    std::unique_ptr<int[]> perm_x;
    std::unique_ptr<int[]> perm_y;

    static std::unique_ptr<int[]> perlinGeneratePerm() {
        auto p = std::make_unique<int[]>(point_count);
        for (int i = 0; i < point_count; ++i) {
            p[i] = i;
        }
        permute(p.get(), point_count);

        return p;
    }

    static void permute(int *p, int n) {
        for (int i = n - 1; i > 0; --i) {
            int target = randomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlinInterp(const std::array<std::array<point2, 2>, 2> &c,
                               double u, double v) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto accum = 0.0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                point2 weight_v = {u - i, v - j};
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) * dot(c[i][j], weight_v);
            }
        }
        return accum;
    }
};
