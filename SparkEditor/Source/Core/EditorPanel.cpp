/**
 * @file EditorPanel.cpp
 * @brief Implementation of the base editor panel class
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorPanel.h"
#include <imgui.h>

namespace SparkEditor {

EditorPanel::EditorPanel(const std::string& name, const std::string& id)
    : m_name(name), m_id(id), m_title(name)
{
}

bool EditorPanel::BeginPanel()
{
    if (!m_isVisible) {
        return false;
    }

    // Create a unique window ID by combining name and ID
    std::string windowName = m_title + "###" + m_id;
    
    // Set window flags
    ImGuiWindowFlags flags = 0;
    if (!m_isClosable) {
        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    }

    // Begin the ImGui window
    bool* pOpen = m_isClosable ? &m_isVisible : nullptr;
    bool isOpen = ImGui::Begin(windowName.c_str(), pOpen, flags);
    
    if (isOpen) {
        // Update panel position and size from ImGui
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        m_posX = pos.x;
        m_posY = pos.y;
        m_width = size.x;
        m_height = size.y;
        
        // Check if this panel is focused
        m_isFocused = ImGui::IsWindowFocused();
    }
    
    return isOpen;
}

void EditorPanel::EndPanel()
{
    ImGui::End();
}

void EditorPanel::NotifyStateChange()
{
    if (m_stateChangeCallback) {
        m_stateChangeCallback(this);
    }
}

} // namespace SparkEditor