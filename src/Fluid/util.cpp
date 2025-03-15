#include "fluid.hpp"

/**
 * Function of dealing with situation with boundary cells.
 * - b : int
 * - x : std::vector<float> &
 */
void Fluid::setBND(int b, std::vector<float> &x) {
    for (int i = 1; i < N - 1; i++) {
        x[IX(i, 0, N)] = b == 2 ? -x[IX(i, 1, N)] : x[IX(i, 1, N)];
        x[IX(i, N - 1, N)] = b == 2 ? -x[IX(i, N - 2, N)] : x[IX(i, N - 2, N)];
    }
    for (int j = 1; j < N - 1; j++) {
        x[IX(0, j, N)] = b == 1 ? -x[IX(1, j, N)] : x[IX(1, j, N)];
        x[IX(N - 1, j, N)] = b == 1 ? -x[IX(N - 2, j, N)] : x[IX(N - 2, j, N)];
    }

    x[IX(0, 0, N)] = 0.5f * (x[IX(1, 0, N)] + x[IX(0, 1, N)]);
    x[IX(0, N - 1, N)] = 0.5f * (x[IX(1, N - 1, N)] + x[IX(0, N - 2, N)]);
    x[IX(N - 1, 0, N)] = 0.5f * (x[IX(N - 2, 0, N)] + x[IX(N - 1, 1, N)]);
    x[IX(N - 1, N - 1, N)] =
        0.5f * (x[IX(N - 2, N - 1, N)] + x[IX(N - 1, N - 2, N)]);
}

/**
 * Function of project: This operation runs through all the cells and fixes
 * them up so everything is in equilibrium.
 * - velocX : std::vector<float> &
 * - velocY : std::vector<float> &
 * - p : std::vector<float> &
 * - div : std::vector<float> &
 */
void Fluid::project(std::vector<float> &velocX, std::vector<float> &velocY,
                    std::vector<float> &p, std::vector<float> &div) {
    for (int j = 1; j < N - 1; ++j) {
        for (int i = 1; i < N - 1; ++i) {
            div[IX(i, j, N)] =
                (-0.5 * (velocX[IX(i + 1, j, N)] - velocX[IX(i - 1, j, N)] +
                         velocY[IX(i, j + 1, N)] - velocY[IX(i, j - 1, N)])) /
                N;
            p[IX(i, j, N)] = 0;
        }
    }

    setBND(0, div);
    setBND(0, p);
    linSolve(0, p, div, 1, 6);

    for (int j = 1; j < N - 1; ++j) {
        for (int i = 1; i < N - 1; ++i) {
            velocX[IX(i, j, N)] -=
                0.5 * (p[IX(i + 1, j, N)] - p[IX(i - 1, j, N)]) * N;
            velocY[IX(i, j, N)] -=
                0.5 * (p[IX(i, j + 1, N)] - p[IX(i, j - 1, N)]) * N;
        }
    }

    setBND(1, velocX);
    setBND(2, velocY);
}

/**
 * Function of advect: responsible for actually moving things around
 * - b : int
 * - d : std::vector<float> &
 * - d0 : const std::vector<float> &
 * - velocX : const std::vector<float> &
 * - velocY : const std::vector<float> &
 * - dt : float
 */
void Fluid::advect(int b, std::vector<float> &d, const std::vector<float> &d0,
                   const std::vector<float> &velocX,
                   const std::vector<float> &velocY, float dt) {
    float i0, i1, j0, j1;

    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = N;
    float ifloat, jfloat;
    int i, j;

    for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
        for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
            tmp1 = dtx * velocX[IX(i, j, N)];
            tmp2 = dty * velocY[IX(i, j, N)];

            x = ifloat - tmp1;
            y = jfloat - tmp2;
            if (x < 0.5f)
                x = 0.5f;
            if (x > Nfloat + 0.5f)
                x = Nfloat + 0.5f;

            i0 = std::floor(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f)
                y = 0.5f;
            if (y > Nfloat + 0.5f)
                y = Nfloat + 0.5f;

            j0 = std::floor(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = static_cast<int>(i0);
            int i1i = static_cast<int>(i1);
            int j0i = static_cast<int>(j0);
            int j1i = static_cast<int>(j1);

            d[IX(i, j, N)] =
                s0 * (t0 * d0[IX(i0i, j0i, N)] + t1 * d0[IX(i0i, j1i, N)]) +
                s1 * (t0 * d0[IX(i1i, j0i, N)] + t1 * d0[IX(i1i, j1i, N)]);
        }
    }

    setBND(b, d);
}

/**
 * Function of solving linear differential equation
 * - b : int
 * - x : std::vector<float> &
 * - x0 : const std::vector<float> &
 * - a : float
 * - c : float
 */
void Fluid::linSolve(int b, std::vector<float> &x, const std::vector<float> &x0,
                     float a, float c) {
    int iter = params.iter;
    float cRecip = 1.0f / c;
    for (int t = 0; t < iter; t++) {
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                x[IX(i, j, N)] =
                    (x0[IX(i, j, N)] +
                     a * (x[IX(i + 1, j, N)] + x[IX(i - 1, j, N)] +
                          x[IX(i, j + 1, N)] + x[IX(i, j - 1, N)])) *
                    cRecip;
            }
        }
        setBND(b, x);
    }
}

/**
 * Function of diffuse
 * - b : int
 * - x : std::vector<float> &
 * - x0 : const std::vector<float> &
 * - diff : float
 * - dt : float
 */
void Fluid::diffuse(int b, std::vector<float> &x, const std::vector<float> &x0,
                    float diff, float dt) {
    float a = dt * diff * (N - 2) * (N - 2);
    linSolve(b, x, x0, a, 1 + 6 * a);
}
