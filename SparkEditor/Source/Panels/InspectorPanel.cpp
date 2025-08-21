/**
 * @file InspectorPanel.cpp
 * @brief Implementation of the Inspector panel
 * @author Spark Engine Team
 * @date 2025
 */

#include "InspectorPanel.h"
#include <imgui.h>
#include <iostream>

namespace SparkEditor {

InspectorPanel::InspectorPanel() 
    : EditorPanel("Inspector", "inspector_panel") {
}

bool InspectorPanel::Initialize() {
    std::cout << "Initializing Inspector panel\n";
    return true;
}

void InspectorPanel::Update(float deltaTime) {
    // Update inspector panel logic
}

void InspectorPanel::Render() {
    if (!IsVisible()) return;

    if (BeginPanel()) {
        if (m_inspectedObject.empty()) {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No object selected");
        } else {
            ImGui::Text("Inspecting: %s", m_inspectedObject.c_str());
            ImGui::Separator();
            
            RenderObjectProperties();
            RenderComponentList();
            
            ImGui::Separator();
            if (ImGui::Button("Add Component")) {
                m_showAddComponentMenu = true;
            }
            
            if (m_showAddComponentMenu) {
                RenderAddComponentMenu();
            }
        }
    }
    EndPanel();
}

void InspectorPanel::Shutdown() {
    std::cout << "Shutting down Inspector panel\n";
}

bool InspectorPanel::HandleEvent(const std::string& eventType, void* eventData) {
    if (eventType == "ObjectSelected" && eventData) {
        SetInspectedObject(*static_cast<std::string*>(eventData));
        return true;
    }
    return false;
}

void InspectorPanel::SetInspectedObject(const std::string& objectId) {
    m_inspectedObject = objectId;
}

void InspectorPanel::RenderObjectProperties() {
    if (ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        char nameBuffer[256];
        strncpy_s(nameBuffer, m_inspectedObject.c_str(), sizeof(nameBuffer) - 1);
        if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
            m_inspectedObject = nameBuffer;
        }
        
        bool active = true;
        ImGui::Checkbox("Active", &active);
        
        // Tag and Layer
        ImGui::Text("Tag: Default");
        ImGui::Text("Layer: Default");
    }
}

void InspectorPanel::RenderComponentList() {
    RenderTransformComponent();
    
    // Render other components
    if (ImGui::CollapsingHeader("Mesh Renderer")) {
        ImGui::Text("Material: Default");
        ImGui::Text("Mesh: Cube");
    }
    
    if (ImGui::CollapsingHeader("Collider")) {
        ImGui::Text("Type: Box Collider");
        bool isTrigger = false;
        ImGui::Checkbox("Is Trigger", &isTrigger);
    }
}

void InspectorPanel::RenderTransformComponent() {
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        float position[3] = {0.0f, 0.0f, 0.0f};
        float rotation[3] = {0.0f, 0.0f, 0.0f};
        float scale[3] = {1.0f, 1.0f, 1.0f};
        
        ImGui::DragFloat3("Position", position, 0.1f);
        ImGui::DragFloat3("Rotation", rotation, 1.0f);
        ImGui::DragFloat3("Scale", scale, 0.1f);
    }
}

void InspectorPanel::RenderAddComponentMenu() {
    if (ImGui::BeginPopup("AddComponentMenu")) {
        if (ImGui::MenuItem("Mesh Renderer")) {
            // Add mesh renderer component
            m_showAddComponentMenu = false;
        }
        if (ImGui::MenuItem("Box Collider")) {
            // Add box collider component
            m_showAddComponentMenu = false;
        }
        if (ImGui::MenuItem("Rigidbody")) {
            // Add rigidbody component
            m_showAddComponentMenu = false;
        }
        ImGui::EndPopup();
    }
    
    if (m_showAddComponentMenu) {
        ImGui::OpenPopup("AddComponentMenu");
    }
}

} // namespace SparkEditor