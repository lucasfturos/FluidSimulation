#pragma once

#include "Common/perlin.hpp"
#include "Common/util.hpp"

#include "Visuals/circle_physics.hpp"
#include "Visuals/density_drawer.hpp"
#include "Visuals/naca_airfoil.hpp"

class Fluid {
  protected:
    const int N = 200;
    const int nSize = N * N;

  private:
    SDL_Surface *surface;

    Uint32 *pixels;

    std::unique_ptr<Perlin> perlin;
    std::unique_ptr<NACA_Airfoil> nacaAirfoil;
    std::unique_ptr<CirclePhysics> circlePhysics;
    std::unique_ptr<DensityDrawer> densityDrawer;

    Vector1f s;
    Vector1f density;
    Vector1f Vx, Vy;
    Vector1f Vx0, Vy0;

    int width, height;
    int mouseX, mouseY;
    int scale;

  private:
    SimulationParams params;
    NACA_AirfoilProfile profile;

    void setupNACA(int, int, int);
    void setupCircle(int, int, int);

    void drawNACA();
    void drawCircle();
    void drawDensity(int);
    void applyMouseInteraction(int &, int &, int);
    void applyRandomInteraction(int, int, int, float &);

    // Logic
    void step();
    void fadeDensity();
    void addDensity(int, int, float);
    void addVelocity(int, int, float, float);
    void addTurbulence(int, int, float, float, float);
    void applyFluidInteraction(int, int, int, float, float, float);

    // Util
    void setBND(int, Vector1f &);
    void project(Vector1f &, Vector1f &, Vector1f &, Vector1f &);
    void linSolve(int, Vector1f &, const Vector1f &, float, float);
    void diffuse(int, Vector1f &x, const Vector1f &, float, float);
    void advect(int, Vector1f &, const Vector1f &, const Vector1f &,
                const Vector1f &, float);

  public:
    Fluid();

    void setMousePos(int, int);
    void setSurface(SDL_Surface *);

    void setSimulationParams(SimulationParams p) { params = p; }
    void setNACA_AirfoilProfile(NACA_AirfoilProfile p) { profile = p; }

    void setup();
    void draw();
};