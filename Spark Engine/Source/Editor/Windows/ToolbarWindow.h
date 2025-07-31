#pragma once
#include "../EditorManager.h"
namespace SparkEngine {
class ToolbarWindow : public EditorWindow {
public:
    void Render() override {
        ImGui::BeginMainMenuBar();
        ImGui::Button("Play"); ImGui::SameLine();
        ImGui::Button("Pause"); ImGui::SameLine();
        ImGui::Button("Step");
        ImGui::EndMainMenuBar();
    }
    const char* GetName() const override { return "Toolbar"; }
};
}
