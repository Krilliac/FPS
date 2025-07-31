#pragma once
#include "../EditorManager.h"
namespace SparkEngine {
class AssetBrowserWindow : public EditorWindow {
public:
    void Render() override {
        ImGui::Begin("Assets");
        // TODO: implement asset tree
        ImGui::End();
    }
    const char* GetName() const override { return "Assets"; }
};
}
