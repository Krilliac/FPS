/**
 * @file SimpleHierarchyPanel.cpp
 * @brief Simple hierarchy panel implementation
 * @author Spark Engine Team
 * @date 2025
 */

#include "SimpleHierarchyPanel.h"
#include <imgui.h>
#include <iostream>

namespace SparkEditor {

SimpleHierarchyPanel::SimpleHierarchyPanel() 
    : EditorPanel("Hierarchy", "simple_hierarchy_panel") {
}

SimpleHierarchyPanel::~SimpleHierarchyPanel() {
}

bool SimpleHierarchyPanel::Initialize() {
    std::cout << "Initializing Simple Hierarchy panel\n";
    
    // Add some default objects
    m_sceneObjects.push_back("Main Camera");
    m_sceneObjects.push_back("Directional Light");
    m_sceneObjects.push_back("Ground Plane");
    m_sceneObjects.push_back("Player");
    
    return true;
}

void SimpleHierarchyPanel::Update(float deltaTime) {
    // Simple hierarchy update
}

void SimpleHierarchyPanel::Render() {
    if (!IsVisible()) return;

    if (BeginPanel()) {
        // Toolbar
        if (ImGui::Button("Create")) {
            ImGui::OpenPopup("CreateObject");
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete") && !m_selectedObject.empty()) {
            DeleteObject(m_selectedObject);
        }
        
        ImGui::Separator();
        
        // Object list
        for (const auto& object : m_sceneObjects) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            
            if (object == m_selectedObject) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            // Object icon
            const char* icon = "??";
            if (object.find("Camera") != std::string::npos) icon = "??";
            else if (object.find("Light") != std::string::npos) icon = "??";
            else if (object.find("Player") != std::string::npos) icon = "??";
            
            ImGui::TreeNodeEx(object.c_str(), flags, "%s %s", icon, object.c_str());
            
            if (ImGui::IsItemClicked()) {
                m_selectedObject = object;
            }
        }
        
        // Create object popup
        if (ImGui::BeginPopup("CreateObject")) {
            if (ImGui::MenuItem("Empty GameObject")) {
                CreateObject("Empty GameObject");
            }
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) CreateObject("Cube");
                if (ImGui::MenuItem("Sphere")) CreateObject("Sphere");
                if (ImGui::MenuItem("Plane")) CreateObject("Plane");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Light")) {
                if (ImGui::MenuItem("Directional Light")) CreateObject("Directional Light");
                if (ImGui::MenuItem("Point Light")) CreateObject("Point Light");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Camera")) CreateObject("Camera");
            ImGui::EndPopup();
        }
    }
    EndPanel();
}

void SimpleHierarchyPanel::Shutdown() {
    std::cout << "Shutting down Simple Hierarchy panel\n";
}

bool SimpleHierarchyPanel::HandleEvent(const std::string& eventType, void* eventData) {
    return false;
}

void SimpleHierarchyPanel::CreateObject(const std::string& objectType) {
    std::string newObjectName = objectType + " " + std::to_string(m_sceneObjects.size() + 1);
    m_sceneObjects.push_back(newObjectName);
    m_selectedObject = newObjectName;
    std::cout << "Created object: " << newObjectName << "\n";
}

void SimpleHierarchyPanel::DeleteObject(const std::string& objectName) {
    auto it = std::find(m_sceneObjects.begin(), m_sceneObjects.end(), objectName);
    if (it != m_sceneObjects.end()) {
        m_sceneObjects.erase(it);
        if (m_selectedObject == objectName) {
            m_selectedObject.clear();
        }
        std::cout << "Deleted object: " << objectName << "\n";
    }
}

const std::string& SimpleHierarchyPanel::GetSelectedObject() const {
    return m_selectedObject;
}

void SimpleHierarchyPanel::SetSelectedObject(const std::string& objectName) {
    m_selectedObject = objectName;
}

} // namespace SparkEditor