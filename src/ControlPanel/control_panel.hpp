#pragma once

#include "Common/util.hpp"
#include "imgui.h"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 300;
    const int widthProfile = width;
    const int heightProfile = 130;
    const int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    const std::vector<const char *> objects = {"Circle", "NACA Airfoil"};

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