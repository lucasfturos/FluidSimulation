#include "control_panel.hpp"

ControlPanel::ControlPanel()
    : params({16, 10, 1.0e-6f, 1.0e-6f, 1.0e-1f, 1, false}) {}

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
    ImGui::InputInt("Scale", &params.scale, 2, 20);
    ImGui::InputFloat("Diffusion", &params.diffusion, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("Viscosity", &params.viscosity, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("dt", &params.dt, 0.0f, 0.0f, "%.1e");
    ImGui::Checkbox("Mouse Interaction", &params.enableMouse);
    ImGui::Combo("Objects", &params.object, objects.data(), objects.size());
    ImGui::PopItemWidth();

    if (ImGui::Button("Reset")) {
        params = {16, 10, 1.0e-6f, 1.0e-6f, 1.0e-1f, 1, false};
    }

    ImGui::Separator();

    ImGui::End();
}