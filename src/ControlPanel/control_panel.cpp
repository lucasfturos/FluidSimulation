#include "control_panel.hpp"

ControlPanel::ControlPanel() : params({16, 10, 1.0e-6f, 1.0e-6f, 1.0e-1f}) {}

SimulationParams ControlPanel::getSimulationParams() { return params; }

void ControlPanel::setup() {
    styleWidget();
    initFont();
}

void ControlPanel::run() {
    ImGui::Begin("Control Panel");
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::Separator();

    ImGui::Text("Simulation Parameters");
    ImGui::PushItemWidth(200);
    ImGui::InputInt("Iterations", &params.iter, 1, 30);
    ImGui::InputFloat("Diffusion", &params.diffusion, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("Viscosity", &params.viscosity, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("dt", &params.dt, 0.0f, 0.0f, "%.1e");
    ImGui::PopItemWidth();
    if (ImGui::Button("Reset")) {
        params = {16, 10, 1.0e-6f, 1.0e-6f, 1.0e-1f};
    }
    
    ImGui::Separator();

    ImGui::End();
}