/**
 * @file SimpleEditorTest.cpp
 * @brief Simple test application for the custom docking system
 * @author Spark Engine Team
 * @date 2025
 */

#include "../Docking/SparkDockingSystem.h"
#include "../Core/EditorPanel.h"
#include <imgui.h>
#include <windows.h>

// Simple test panel
class TestPanel : public SparkEditor::EditorPanel {
public:
    TestPanel(const std::string& name) : EditorPanel(name, name + "_ID") {}
    
    bool Initialize() override { return true; }
    void Update(float deltaTime) override {}
    void Render() override {
        ImGui::Text("This is test panel: %s", GetName().c_str());
        ImGui::Button("Test Button");
    }
};

class DockingTestApp {
public:
    bool Initialize() {
        // Initialize docking system
        m_dockingSystem = std::make_unique<SparkEditor::SparkDockingSystem>();
        if (!m_dockingSystem->Initialize()) {
            return false;
        }
        
        // Create test panels
        auto panel1 = std::make_shared<TestPanel>("Scene Graph");
        auto panel2 = std::make_shared<TestPanel>("Entity Inspector");
        auto panel3 = std::make_shared<TestPanel>("Asset Explorer");
        auto panel4 = std::make_shared<TestPanel>("Engine Console");
        
        // Register panels with docking system
        m_dockingSystem->RegisterPanel(panel1);
        m_dockingSystem->RegisterPanel(panel2);
        m_dockingSystem->RegisterPanel(panel3);
        m_dockingSystem->RegisterPanel(panel4);
        
        // Set up initial layout
        m_dockingSystem->DockPanel(panel1->GetID(), SparkEditor::DockPosition::Left);
        m_dockingSystem->DockPanel(panel2->GetID(), SparkEditor::DockPosition::Right);
        m_dockingSystem->DockPanel(panel3->GetID(), SparkEditor::DockPosition::Center);
        m_dockingSystem->DockPanel(panel4->GetID(), SparkEditor::DockPosition::Bottom);
        
        m_panels.push_back(panel1);
        m_panels.push_back(panel2);
        m_panels.push_back(panel3);
        m_panels.push_back(panel4);
        
        return true;
    }
    
    void Update(float deltaTime) {
        if (m_dockingSystem) {
            m_dockingSystem->Update(deltaTime);
        }
        
        for (auto& panel : m_panels) {
            panel->Update(deltaTime);
        }
    }
    
    void Render() {
        if (m_dockingSystem) {
            m_dockingSystem->BeginFrame();
        }
        
        // Render main menu
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Default Layout")) {
                    m_dockingSystem->ApplyPredefinedLayout("Default");
                }
                if (ImGui::MenuItem("Code Layout")) {
                    m_dockingSystem->ApplyPredefinedLayout("Code");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        
        // Render demo window for comparison
        static bool showDemo = true;
        if (showDemo) {
            ImGui::ShowDemoWindow(&showDemo);
        }
        
        if (m_dockingSystem) {
            m_dockingSystem->EndFrame();
        }
    }
    
    void Shutdown() {
        if (m_dockingSystem) {
            m_dockingSystem->Shutdown();
            m_dockingSystem.reset();
        }
        m_panels.clear();
    }
    
private:
    std::unique_ptr<SparkEditor::SparkDockingSystem> m_dockingSystem;
    std::vector<std::shared_ptr<TestPanel>> m_panels;
};

// Example of how to use the docking system
int TestSparkDockingSystem() {
    DockingTestApp app;
    
    if (!app.Initialize()) {
        return -1;
    }
    
    // Main loop would go here
    float deltaTime = 1.0f/60.0f;
    
    for (int frame = 0; frame < 1000; ++frame) {
        app.Update(deltaTime);
        app.Render();
    }
    
    app.Shutdown();
    return 0;
}