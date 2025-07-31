#pragma once
#include "../EditorManager.h"
namespace SparkEngine {
class DockSpaceWindow : public EditorWindow {
public:
    void Render() override { ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()); }
    const char* GetName() const override { return "DockSpace"; }
};
}
