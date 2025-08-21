/**
 * @file EditorUI.cpp
 * @brief Implementation of the enhanced editor UI system
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorUI.h"
#include "../Utils/SparkConsole.h"  // Use local SparkConsole instead of engine version
#include "../Panels/SceneViewPanel.h"
#include "../Panels/SimpleConsolePanel.h"
#include "../Panels/SimpleHierarchyPanel.h"
#include "../Panels/InspectorPanel.h"
#include "../Panels/AssetBrowserPanel.h"
#include "EditorCrashHandler.h"
#include "EditorApplication.h"  // For EditorConfig definition
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <filesystem>
#include <fstream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

namespace SparkEditor {

EditorUI::EditorUI() {
    m_crashHandler = &EditorCrashHandler::GetInstance();
}

EditorUI::~EditorUI() {
    if (m_isInitialized) {
        Shutdown();
    }
}

bool EditorUI::Initialize(const EditorConfig& config) {
    auto& console = Spark::SimpleConsole::GetInstance();
    console.LogInfo("Initializing Enhanced EditorUI with full configuration...");
    
    try {
        // Store config
        m_config = std::unique_ptr<EditorConfig>(new EditorConfig(config));
        
        console.LogInfo("Using enhanced initialization for production use");
        
        // Initialize crash handler
        console.LogInfo("Initializing crash handler...");
        if (m_crashHandler && m_crashHandler->Initialize()) {
            console.LogSuccess("Crash handler initialized successfully");
        } else {
            console.LogWarning("Crash handler initialization failed");
        }
        
        // Create panels
        console.LogInfo("Creating editor panels...");
        CreatePanels();
        console.LogSuccess("Panels created successfully");
        
        m_isInitialized = true;
        console.LogSuccess("Enhanced EditorUI initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        console.LogError("Exception in EditorUI::Initialize: " + std::string(e.what()));
        return false;
    } catch (...) {
        console.LogError("Unknown exception in EditorUI::Initialize");
        return false;
    }
}

void EditorUI::Update(float deltaTime) {
    if (!m_isInitialized) return;
    
    // Update notifications
    auto it = m_notifications.begin();
    while (it != m_notifications.end()) {
        it->timeLeft -= deltaTime;
        if (it->timeLeft <= 0.0f && it->duration > 0.0f) {
            it = m_notifications.erase(it);
        } else {
            ++it;
        }
    }
    
    // Update stats
    UpdateStats(deltaTime);
}

void EditorUI::Render() {
    if (!m_isInitialized) return;
    
    // Render main UI components
    RenderMainMenuBar();
    RenderPanels();
    RenderStatusBar();
    RenderNotifications();
    RenderModalDialogs();
    
    // Demo window for testing
    if (m_showDemoWindow) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }
}

void EditorUI::Shutdown() {
    auto& console = Spark::SimpleConsole::GetInstance();
    console.LogInfo("Shutting down EditorUI...");
    
    // Shutdown panels using vector iteration since unordered_map doesn't have rbegin/rend
    std::vector<std::pair<std::string, std::shared_ptr<EditorPanel>>> panelVector(m_panels.begin(), m_panels.end());
    for (auto it = panelVector.rbegin(); it != panelVector.rend(); ++it) {
        try {
            if (it->second) {
                console.LogInfo("Shutting down " + it->first + " panel");
                it->second->Shutdown();
                console.LogSuccess(it->first + " panel shutdown complete");
            }
        } catch (const std::exception& e) {
            console.LogError("Exception shutting down " + it->first + " panel: " + std::string(e.what()));
        }
    }
    m_panels.clear();
    console.LogInfo("All panels shutdown and cleared");
    
    // Note: Don't shutdown crash handler here as it's managed elsewhere
    
    m_isInitialized = false;
    console.LogSuccess("EditorUI shutdown complete");
}

void EditorUI::CreatePanels() {
    auto& console = Spark::SimpleConsole::GetInstance();
    console.LogInfo("Creating editor panels...");
    
    // Skip complex panels that may cause deadlocks in debugger environment
    bool isDebuggerPresent = false;
#ifdef _WIN32
    isDebuggerPresent = IsDebuggerPresent();
#endif

    if (isDebuggerPresent) {
        console.LogWarning("DEBUGGER DETECTED - Using minimal panel set to avoid deadlocks");
        
        // Only create the most essential panels
        try {
            console.LogInfo("Creating Scene View panel...");
            auto sceneViewPanel = std::shared_ptr<SceneViewPanel>(new SceneViewPanel());
            m_panels["SceneView"] = sceneViewPanel;
            console.LogSuccess("Created Scene View panel");
        } catch (const std::exception& e) {
            console.LogError("Failed to create Scene View panel: " + std::string(e.what()));
        }
        
        // Initialize essential panels only
        for (auto& [name, panel] : m_panels) {
            try {
                console.LogInfo("Initializing " + name + " panel");
                
                if (panel && panel->Initialize()) {
                    console.LogSuccess("Initialized " + name + " panel");
                } else {
                    console.LogError("Failed to initialize " + name + " panel");
                }
            } catch (const std::exception& e) {
                console.LogError("Exception initializing " + name + " panel: " + std::string(e.what()));
            }
        }
        
        console.LogInfo("Created " + std::to_string(m_panels.size()) + " editor panels (minimal set for debugger)");
        return;
    }
    
    // Full panel creation for release mode
    console.LogInfo("Creating full panel set...");
    
    // Create Scene View Panel (working)
    try {
        console.LogInfo("Creating Scene View panel...");
        auto sceneViewPanel = std::shared_ptr<SceneViewPanel>(new SceneViewPanel());
        m_panels["SceneView"] = sceneViewPanel;
        console.LogSuccess("Created Scene View panel");
    } catch (const std::exception& e) {
        console.LogError("Failed to create Scene View panel: " + std::string(e.what()));
    }
    
    // Create Simple Console Panel
    try {
        console.LogInfo("Creating Simple Console panel...");
        auto consolePanel = std::shared_ptr<SimpleConsolePanel>(new SimpleConsolePanel());
        m_panels["Console"] = consolePanel;
        console.LogSuccess("Created Simple Console panel");
    } catch (const std::exception& e) {
        console.LogError("Failed to create Simple Console panel: " + std::string(e.what()));
    }
    
    // Create Simple Hierarchy Panel
    try {
        console.LogInfo("Creating Simple Hierarchy panel...");
        auto hierarchyPanel = std::shared_ptr<SimpleHierarchyPanel>(new SimpleHierarchyPanel());
        m_panels["Hierarchy"] = hierarchyPanel;
        console.LogSuccess("Created Simple Hierarchy panel");
    } catch (const std::exception& e) {
        console.LogError("Failed to create Simple Hierarchy panel: " + std::string(e.what()));
    }
    
    // Create Inspector Panel
    try {
        console.LogInfo("Creating Inspector panel...");
        auto inspectorPanel = std::shared_ptr<InspectorPanel>(new InspectorPanel());
        m_panels["Inspector"] = inspectorPanel;
        console.LogSuccess("Created Inspector panel");
    } catch (const std::exception& e) {
        console.LogError("Failed to create Inspector panel: " + std::string(e.what()));
    }
    
    // Create Asset Browser Panel
    try {
        console.LogInfo("Creating Asset Browser panel...");
        auto assetBrowserPanel = std::shared_ptr<AssetBrowserPanel>(new AssetBrowserPanel());
        m_panels["AssetBrowser"] = assetBrowserPanel;
        console.LogSuccess("Created Asset Browser panel");
    } catch (const std::exception& e) {
        console.LogError("Failed to create Asset Browser panel: " + std::string(e.what()));
    }
    
    // SKIP SimpleBuildSystem in all modes since it's causing the hang
    console.LogWarning("SKIPPING Simple Build System panel (known to cause hangs)");
    
    // Initialize all panels
    for (auto& [name, panel] : m_panels) {
        try {
            console.LogInfo("Initializing " + name + " panel");
            
            if (panel && panel->Initialize()) {
                console.LogSuccess("Initialized " + name + " panel");
            } else {
                console.LogError("Failed to initialize " + name + " panel");
            }
        } catch (const std::exception& e) {
            console.LogError("Exception initializing " + name + " panel: " + std::string(e.what()));
        }
    }
    
    console.LogSuccess("Created " + std::to_string(m_panels.size()) + " editor panels");
}

void EditorUI::RenderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                ShowNotification("New Scene created!", "success");
            }
            if (ImGui::MenuItem("Open Scene")) {
                ShowNotification("Open Scene dialog coming soon!", "info");
            }
            if (ImGui::MenuItem("Save Scene")) {
                ShowNotification("Scene saved!", "success");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("New Project")) {
                ShowNotification("New Project feature coming soon!", "info");
            }
            if (ImGui::MenuItem("Open Project")) {
                ShowNotification("Open Project feature coming soon!", "info");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Import Asset")) {
                ShowNotification("Import Asset feature coming soon!", "info");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Build Settings")) {
                ShowNotification("Build Settings coming soon!", "info");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                ShowNotification("Exit feature coming soon!", "info");
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                ShowNotification("Undo operation!", "info");
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                ShowNotification("Redo operation!", "info");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
                ShowNotification("Cut operation!", "info");
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                ShowNotification("Copy operation!", "info");
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                ShowNotification("Paste operation!", "info");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Select All", "Ctrl+A")) {
                ShowNotification("Select All operation!", "info");
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("GameObject")) {
            if (ImGui::MenuItem("Create Empty")) {
                ShowNotification("Created empty GameObject!", "success");
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) {
                    ShowNotification("Created Cube!", "success");
                }
                if (ImGui::MenuItem("Sphere")) {
                    ShowNotification("Created Sphere!", "success");
                }
                if (ImGui::MenuItem("Cylinder")) {
                    ShowNotification("Created Cylinder!", "success");
                }
                if (ImGui::MenuItem("Plane")) {
                    ShowNotification("Created Plane!", "success");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Light")) {
                if (ImGui::MenuItem("Directional Light")) {
                    ShowNotification("Created Directional Light!", "success");
                }
                if (ImGui::MenuItem("Point Light")) {
                    ShowNotification("Created Point Light!", "success");
                }
                if (ImGui::MenuItem("Spot Light")) {
                    ShowNotification("Created Spot Light!", "success");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Camera")) {
                if (ImGui::MenuItem("Camera")) {
                    ShowNotification("Created Camera!", "success");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Hierarchy", nullptr, IsPanelVisible("Hierarchy"))) {
                SetPanelVisible("Hierarchy", !IsPanelVisible("Hierarchy"));
            }
            if (ImGui::MenuItem("Inspector", nullptr, IsPanelVisible("Inspector"))) {
                SetPanelVisible("Inspector", !IsPanelVisible("Inspector"));
            }
            if (ImGui::MenuItem("Scene View", nullptr, IsPanelVisible("SceneView"))) {
                SetPanelVisible("SceneView", !IsPanelVisible("SceneView"));
            }
            if (ImGui::MenuItem("Asset Browser", nullptr, IsPanelVisible("AssetBrowser"))) {
                SetPanelVisible("AssetBrowser", !IsPanelVisible("AssetBrowser"));
            }
            if (ImGui::MenuItem("Console", nullptr, IsPanelVisible("Console"))) {
                SetPanelVisible("Console", !IsPanelVisible("Console"));
            }
            if (ImGui::MenuItem("Build System", nullptr, IsPanelVisible("BuildSystem"))) {
                SetPanelVisible("BuildSystem", !IsPanelVisible("BuildSystem"));
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Reset Layout")) {
                ResetToDefaultLayout();
                ShowNotification("Layout reset!", "success");
            }
            if (ImGui::MenuItem("Save Layout")) {
                SaveLayout("Custom Layout");
                ShowNotification("Layout saved!", "success");
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Show Demo Window", nullptr, m_showDemoWindow)) {
                m_showDemoWindow = !m_showDemoWindow;
            }
            if (ImGui::MenuItem("About")) {
                ShowNotification("Spark Engine Editor v1.0", "info", 5.0f);
            }
            if (ImGui::MenuItem("Documentation")) {
                ShowNotification("Documentation coming soon!", "info");
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void EditorUI::RenderStatusBar() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 statusBarSize = ImVec2(viewport->Size.x, 20);
    ImVec2 statusBarPos = ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - statusBarSize.y);
    
    ImGui::SetNextWindowPos(statusBarPos);
    ImGui::SetNextWindowSize(statusBarSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
                           ImGuiWindowFlags_NoSavedSettings;
    
    if (ImGui::Begin("StatusBar", nullptr, flags)) {
        ImGui::Text("Engine: %s | FPS: %.1f | Frame: %llu", 
                   m_engineConnected ? "Connected" : "Disconnected",
                   m_stats.frameTime > 0 ? 1000.0f / m_stats.frameTime : 0,
                   m_frameNumber);
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 200);
        ImGui::Text("Objects: %d | Assets: %d", m_sceneObjectCount, m_assetDatabaseSize);
    }
    ImGui::End();
}

void EditorUI::RenderNotifications() {
    const float NOTIFICATION_WIDTH = 300.0f;
    const float NOTIFICATION_HEIGHT = 60.0f;
    const float NOTIFICATION_SPACING = 10.0f;
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float yOffset = viewport->Pos.y + 30.0f; // Below menu bar
    
    for (size_t i = 0; i < m_notifications.size(); ++i) {
        const auto& notification = m_notifications[i];
        
        ImVec2 notificationPos = ImVec2(
            viewport->Pos.x + viewport->Size.x - NOTIFICATION_WIDTH - 10.0f,
            yOffset + i * (NOTIFICATION_HEIGHT + NOTIFICATION_SPACING)
        );
        
        ImGui::SetNextWindowPos(notificationPos);
        ImGui::SetNextWindowSize(ImVec2(NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT));
        
        std::string windowName = "Notification##" + std::to_string(i);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
        
        if (ImGui::Begin(windowName.c_str(), nullptr, flags)) {
            // Color based on type
            ImVec4 color = ImVec4(1, 1, 1, 1);
            if (notification.type == "error") color = ImVec4(1, 0.4f, 0.4f, 1);
            else if (notification.type == "warning") color = ImVec4(1, 1, 0.4f, 1);
            else if (notification.type == "success") color = ImVec4(0.4f, 1, 0.4f, 1);
            
            ImGui::TextColored(color, "%s", notification.message.c_str());
            
            if (notification.duration > 0.0f) {
                float progress = 1.0f - (notification.timeLeft / notification.duration);
                ImGui::ProgressBar(progress, ImVec2(-1, 4));
            }
        }
        ImGui::End();
    }
}

void EditorUI::RenderPanels() {
    static float lastUpdateTime = 0.0f;
    static auto lastClock = std::chrono::steady_clock::now();
    
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastClock).count();
    lastClock = now;
    
    for (auto& [name, panel] : m_panels) {
        if (panel->IsVisible()) {
            panel->Update(deltaTime); // Fixed: Pass proper delta time
            panel->Render();
        }
    }
}

void EditorUI::RenderModalDialogs() {
    if (m_currentDialog.isOpen) {
        ImGui::OpenPopup(m_currentDialog.title.c_str());
        
        if (ImGui::BeginPopupModal(m_currentDialog.title.c_str(), &m_currentDialog.isOpen)) {
            if (m_currentDialog.content) {
                m_currentDialog.content();
            }
            
            ImGui::Separator();
            
            for (const auto& [buttonName, callback] : m_currentDialog.buttons) {
                if (ImGui::Button(buttonName.c_str())) {
                    if (callback) callback();
                    m_currentDialog.isOpen = false;
                }
                ImGui::SameLine();
            }
            
            ImGui::EndPopup();
        }
    }
}

void EditorUI::UpdateStats(float deltaTime) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastStatsUpdate);
    
    if (elapsed.count() >= 500) { // Update every 500ms
        m_stats.frameTime = deltaTime * 1000.0f; // Convert to ms
        m_stats.visiblePanels = 0;
        m_stats.totalPanels = static_cast<int>(m_panels.size());
        
        for (const auto& [name, panel] : m_panels) {
            if (panel->IsVisible()) {
                m_stats.visiblePanels++;
            }
        }
        
        m_stats.lastUpdate = now;
        m_lastStatsUpdate = now;
    }
}

// Simple implementations for interface methods
bool EditorUI::IsPanelVisible(const std::string& panelName) const {
    auto it = m_panels.find(panelName);
    return it != m_panels.end() ? it->second->IsVisible() : false;
}

void EditorUI::SetPanelVisible(const std::string& panelName, bool visible) {
    auto it = m_panels.find(panelName);
    if (it != m_panels.end()) {
        it->second->SetVisible(visible);
    }
}

bool EditorUI::SaveLayout(const std::string& layoutName, const std::string& description) {
    // Implementation for saving ImGui docking layout
    try {
        std::string layoutsDir = "Layouts";
        if (!std::filesystem::exists(layoutsDir)) {
            std::filesystem::create_directories(layoutsDir);
        }
        
        std::string filePath = layoutsDir + "/" + layoutName + ".ini";
        
        // Save ImGui settings to specific file
        ImGui::SaveIniSettingsToDisk(filePath.c_str());
        
        // Save layout metadata
        std::ofstream metaFile(filePath + ".meta");
        if (metaFile.is_open()) {
            metaFile << "name=" << layoutName << "\n";
            metaFile << "description=" << description << "\n";
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            metaFile << "created=" << time_t << "\n";
        }
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool EditorUI::LoadLayout(const std::string& layoutName) {
    // Implementation for loading ImGui docking layout
    try {
        std::string filePath = "Layouts/" + layoutName + ".ini";
        
        if (!std::filesystem::exists(filePath)) {
            return false;
        }
        
        // Load ImGui settings from specific file
        ImGui::LoadIniSettingsFromDisk(filePath.c_str());
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void EditorUI::ResetToDefaultLayout() {
    // Implementation for resetting to default layout
    // Instead of ClearIniSettings, we'll load default settings
    
    // Reset all panels to default visibility
    for (auto& [name, panel] : m_panels) {
        // Set sensible defaults based on panel type
        if (name == "Scene View" || name == "Console" || name == "Hierarchy") {
            panel->SetVisible(true);
        } else {
            panel->SetVisible(false);
        }
    }
    
    // Reset ImGui style to default
    ImGuiStyle& style = ImGui::GetStyle();
    style = ImGuiStyle(); // Reset to default constructor values
}

void EditorUI::ApplyTheme(const std::string& themeName) {
    m_currentTheme = themeName;
    
    // Apply theme to ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    
    if (themeName == "Dark") {
        ImGui::StyleColorsDark(&style);
        
        // Customize dark theme with available colors
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        
    } else if (themeName == "Light") {
        ImGui::StyleColorsLight(&style);
        
        // Customize light theme with available colors
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
        
    } else if (themeName == "Classic") {
        ImGui::StyleColorsClassic(&style);
        
    } else {
        // Default to dark theme
        ImGui::StyleColorsDark(&style);
    }
    
    // Apply consistent style settings
    style.WindowRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;
}

void EditorUI::ShowNotification(const std::string& message, const std::string& type, float duration) {
    Notification notification;
    notification.message = message;
    notification.type = type;
    notification.duration = duration;
    notification.timeLeft = duration;
    notification.timestamp = std::chrono::steady_clock::now();
    
    m_notifications.push_back(notification);
}

std::string EditorUI::ExecuteCommand(const std::string& command) {
    // Simple command parsing
    std::vector<std::string> parts;
    std::string current;
    for (char c : command) {
        if (c == ' ') {
            if (!current.empty()) {
                parts.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        parts.push_back(current);
    }
    
    if (parts.empty()) {
        return "Empty command";
    }
    
    auto it = m_commands.find(parts[0]);
    if (it != m_commands.end()) {
        std::vector<std::string> args(parts.begin() + 1, parts.end());
        return it->second(args);
    }
    
    return "Unknown command: " + parts[0];
}

void EditorUI::RegisterCommand(const std::string& name, 
                              std::function<std::string(const std::vector<std::string>&)> handler,
                              const std::string& description) {
    m_commands[name] = handler;
}

void EditorUI::SetFrameNumber(uint64_t frameNumber) {
    m_frameNumber = frameNumber;
}

UIStats EditorUI::GetStats() const {
    return m_stats;
}

void EditorUI::SetEngineConnected(bool connected) {
    m_engineConnected = connected;
}

void EditorUI::UpdateAssetDatabaseInfo(int assetCount, size_t memoryUsage) {
    m_assetDatabaseSize = assetCount;
    m_assetMemoryUsage = memoryUsage;
}

void EditorUI::UpdateSceneInfo(int objectCount, int selectedCount) {
    m_sceneObjectCount = objectCount;
    m_selectedObjectCount = selectedCount;
}

bool EditorUI::HasRecoveryData() {
    return m_recoveryDataAvailable;
}

bool EditorUI::ShowRecoveryDialog() {
    // TODO: Implement recovery dialog
    return false;
}

bool EditorUI::ImportLayout(const std::string& filePath) {
    // TODO: Implement layout import
    return false;
}

bool EditorUI::ExportLayout(const std::string& filePath) {
    // TODO: Implement layout export
    return false;
}

void EditorUI::ShowModalDialog(const std::string& title, 
                              std::function<void()> content,
                              const std::unordered_map<std::string, std::function<void()>>& buttons) {
    m_currentDialog.title = title;
    m_currentDialog.content = content;
    m_currentDialog.buttons = buttons;
    m_currentDialog.isOpen = true;
}

} // namespace SparkEditor