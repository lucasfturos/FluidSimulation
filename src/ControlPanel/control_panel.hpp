#pragma once

#include "imgui.h"
#include "Common/util.hpp"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 270;

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