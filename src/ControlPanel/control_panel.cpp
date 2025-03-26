#include "control_panel.hpp"

ControlPanel::ControlPanel() : params(defaultParams), profile(defaultProfile) {}

SimulationParams ControlPanel::getSimulationParams() { return params; }

NACA_AirfoilProfile ControlPanel::getNACA_AirfoilProfile() { return profile; }

void ControlPanel::setup() {
    styleWidget();
    initFont();
}

void ControlPanel::mainWindow() {
    ImGui::Begin("Control Panel", nullptr, flags);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(width, height));

    ImVec2 mainPos = ImGui::GetWindowPos();
    ImVec2 mainSize = ImGui::GetWindowSize();
    bottomMainHeight = mainPos.y + mainSize.y;

    ImGui::Separator();

    ImGui::Text("Simulation Parameters");

    ImGui::PushItemWidth(200);
    ImGui::InputInt("Iterations", &params.iter, 1, 30);
    ImGui::InputInt("Scale", &params.scale, 2, 20);
    ImGui::InputFloat("Diffusion", &params.diffusion, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("Viscosity", &params.viscosity, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("dt", &params.dt, 0.0f, 0.0f, "%.1e");
    ImGui::InputFloat("Fade Rate", &params.fadeRate, 0.0f, 0.0f, "%.1e");
    ImGui::Combo("Objects", &params.object, objects.data(), objects.size());
    ImGui::Checkbox("Mouse Interaction", &params.enableMouse);
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        params = defaultParams;
    }
    ImGui::PopItemWidth();

    ImGui::Separator();

    ImGui::End();
}

void ControlPanel::nacaWindow() {
    ImGui::Begin("Defines the NACA Airfoil Profile", nullptr, flags);
    ImGui::SetWindowPos(ImVec2(0, bottomMainHeight + 1));
    ImGui::SetWindowSize(ImVec2(widthProfile, heightProfile));

    ImGui::PushItemWidth(180);
    ImGui::InputFloat("Camber", &profile.camber, 0.1f, 1.0f, "%.1f",
                      ImGuiInputTextFlags_CharsDecimal);
    profile.camber = (profile.camber < 0.0f)   ? 0.0f
                     : (profile.camber > 9.5f) ? 9.5f
                                               : profile.camber;

    ImGui::InputFloat("Max Camber", &profile.maxCamber, 0.1f, 1.0f, "%.1f",
                      ImGuiInputTextFlags_CharsDecimal);
    profile.maxCamber = (profile.maxCamber < 0.0f)    ? 0.0f
                        : (profile.maxCamber > 90.0f) ? 90.0f
                                                      : profile.maxCamber;

    ImGui::InputFloat("Thickness", &profile.thickness, 0.1f, 1.0f, "%.1f",
                      ImGuiInputTextFlags_CharsDecimal);
    profile.thickness = (profile.thickness < 1.0f)    ? 1.0f
                        : (profile.thickness > 40.0f) ? 40.0f
                                                      : profile.thickness;
    ImGui::PopItemWidth();

    if (ImGui::Button("Reset")) {
        profile = defaultProfile;
    }

    ImGui::Separator();

    ImGui::End();
}

void ControlPanel::run() {
    mainWindow();
    if (params.object == 1)
        nacaWindow();
}