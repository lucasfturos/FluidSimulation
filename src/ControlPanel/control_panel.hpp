#pragma once

#include "Common/util.hpp"
#include "imgui.h"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 300;

    const std::vector<const char *> objects = {"Circle", "NACA"};

  private:
    SimulationParams params;

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    SimulationParams getSimulationParams();

    void setup();
    void run();
};