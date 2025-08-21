/**
 * @file AssetBrowserPanel.cpp
 * @brief Implementation of the Asset Browser panel
 * @author Spark Engine Team
 * @date 2025
 */

#include "AssetBrowserPanel.h"
#include <imgui.h>
#include <iostream>
#include <filesystem>

namespace SparkEditor {

AssetBrowserPanel::AssetBrowserPanel() 
    : EditorPanel("Asset Browser", "asset_browser_panel") {
}

bool AssetBrowserPanel::Initialize() {
    std::cout << "Initializing Asset Browser panel\n";
    return true;
}

void AssetBrowserPanel::Update(float deltaTime) {
    // Update asset browser logic
}

void AssetBrowserPanel::Render() {
    if (!IsVisible()) return;

    if (BeginPanel()) {
        // Toolbar
        if (ImGui::Button("Import")) {
            // Open file dialog for importing assets
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            RefreshAssets();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Size", &m_thumbnailSize, 32.0f, 128.0f);
        
        ImGui::Separator();
        
        // Split view: folder tree on left, asset grid on right
        if (ImGui::BeginTable("AssetBrowserTable", 2, ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("Folders", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableSetupColumn("Assets", ImGuiTableColumnFlags_WidthStretch);
            
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            RenderFolderTree();
            
            ImGui::TableSetColumnIndex(1);
            RenderAssetGrid();
            
            ImGui::EndTable();
        }
        
        ImGui::Separator();
        RenderAssetDetails();
    }
    EndPanel();
}

void AssetBrowserPanel::Shutdown() {
    std::cout << "Shutting down Asset Browser panel\n";
}

bool AssetBrowserPanel::HandleEvent(const std::string& eventType, void* eventData) {
    return false;
}

void AssetBrowserPanel::SetProjectPath(const std::string& projectPath) {
    m_projectPath = projectPath;
    m_currentFolder = projectPath;
    RefreshAssets();
}

void AssetBrowserPanel::RenderFolderTree() {
    ImGui::BeginChild("FolderTree");
    
    if (ImGui::TreeNodeEx("Assets", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Render folder hierarchy
        try {
            if (!m_projectPath.empty() && std::filesystem::exists(m_projectPath)) {
                for (const auto& entry : std::filesystem::directory_iterator(m_projectPath)) {
                    if (entry.is_directory()) {
                        std::string folderName = entry.path().filename().string();
                        if (ImGui::TreeNode(folderName.c_str())) {
                            if (ImGui::IsItemClicked()) {
                                m_currentFolder = entry.path().string();
                                RefreshAssets();
                            }
                            ImGui::TreePop();
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "Error reading folders");
        }
        
        ImGui::TreePop();
    }
    
    ImGui::EndChild();
}

void AssetBrowserPanel::RenderAssetGrid() {
    ImGui::BeginChild("AssetGrid");
    
    // Current path
    ImGui::Text("Path: %s", m_currentFolder.c_str());
    ImGui::Separator();
    
    // Asset grid
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = std::max(1, (int)(panelWidth / (m_thumbnailSize + 10.0f)));
    
    if (ImGui::BeginTable("AssetGridTable", columns)) {
        int itemIndex = 0;
        for (const auto& asset : m_assets) {
            if (itemIndex % columns == 0) {
                ImGui::TableNextRow();
            }
            ImGui::TableSetColumnIndex(itemIndex % columns);
            
            // Asset thumbnail (placeholder)
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 size(m_thumbnailSize, m_thumbnailSize);
            
            bool isSelected = (asset == m_selectedAsset);
            ImU32 borderColor = isSelected ? IM_COL32(100, 150, 255, 255) : IM_COL32(100, 100, 100, 255);
            
            drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(80, 80, 80, 255));
            drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), borderColor);
            
            // Asset icon (simple)
            ImVec2 iconPos = ImVec2(pos.x + size.x * 0.25f, pos.y + size.y * 0.25f);
            ImVec2 iconSize = ImVec2(size.x * 0.5f, size.y * 0.5f);
            drawList->AddRectFilled(iconPos, ImVec2(iconPos.x + iconSize.x, iconPos.y + iconSize.y), 
                                   IM_COL32(150, 150, 150, 255));
            
            // Handle click
            ImGui::SetCursorScreenPos(pos);
            ImGui::InvisibleButton(asset.c_str(), size);
            if (ImGui::IsItemClicked()) {
                m_selectedAsset = asset;
            }
            
            // Asset name
            ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + size.y + 2.0f));
            std::string filename = std::filesystem::path(asset).filename().string();
            if (filename.length() > 12) {
                filename = filename.substr(0, 9) + "...";
            }
            ImGui::Text("%s", filename.c_str());
            
            itemIndex++;
        }
        ImGui::EndTable();
    }
    
    ImGui::EndChild();
}

void AssetBrowserPanel::RenderAssetDetails() {
    ImGui::BeginChild("AssetDetails", ImVec2(0, 100));
    
    if (!m_selectedAsset.empty()) {
        ImGui::Text("Selected: %s", std::filesystem::path(m_selectedAsset).filename().string().c_str());
        ImGui::Text("Path: %s", m_selectedAsset.c_str());
        
        try {
            if (std::filesystem::exists(m_selectedAsset)) {
                auto fileSize = std::filesystem::file_size(m_selectedAsset);
                ImGui::Text("Size: %lld bytes", fileSize);
                
                auto writeTime = std::filesystem::last_write_time(m_selectedAsset);
                ImGui::Text("Modified: [File timestamp]");
            }
        } catch (const std::exception& e) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "Error reading file info");
        }
    } else {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No asset selected");
    }
    
    ImGui::EndChild();
}

void AssetBrowserPanel::RefreshAssets() {
    m_assets.clear();
    
    try {
        if (!m_currentFolder.empty() && std::filesystem::exists(m_currentFolder)) {
            for (const auto& entry : std::filesystem::directory_iterator(m_currentFolder)) {
                if (entry.is_regular_file()) {
                    m_assets.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error refreshing assets: " << e.what() << std::endl;
    }
}

void AssetBrowserPanel::ImportAsset(const std::string& filePath) {
    // TODO: Implement asset import logic
    std::cout << "Importing asset: " << filePath << std::endl;
}

} // namespace SparkEditor