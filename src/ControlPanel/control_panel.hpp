#pragma once

#include "Common/util.hpp"
#include "imgui.h"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 290;
    const int widthProfile = width;
    const int heightProfile = 155;
    const int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    const std::vector<const char *> objects = {"Circle", "NACA Airfoil"};

    const SimulationParams defaultParams{
        .iter = 3,
        .scale = 10,
        .diffusion = 1.0e-6f,
        .viscosity = 1.0e-6f,
        .dt = 1.0e-1f,
        .fadeRate = 0.02,
        .object = 0,
        .enableMouse = false,
    };
    const NACA_AirfoilProfile defaultProfile{
        .camber = 2.0f,
        .maxCamber = 50.0f,
        .thickness = 12.0f,
    };

  private:
    float bottomMainHeight;

    SimulationParams params;
    NACA_AirfoilProfile profile;

    void mainWindow();
    void nacaWindow();

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    SimulationParams getSimulationParams();
    NACA_AirfoilProfile getNACA_AirfoilProfile();

    void setup();
    void run();
};