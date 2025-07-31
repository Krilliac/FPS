#pragma once
#include "../EditorManager.h"
#include "../Graphics/GraphicsEngine.h"
namespace SparkEngine {
class ViewportWindow : public EditorWindow {
    GraphicsEngine* m_graphics;
public:
    ViewportWindow(GraphicsEngine* g) : m_graphics(g) {}
    void Render() override {
        ImGui::Begin("Viewport");
        ImVec2 sz = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)m_graphics->GetRenderTargetSRV(), sz);
        ImGui::End();
    }
    const char* GetName() const override { return "Viewport"; }
};
}
