/**
 * @file EditorUI.cpp
 * @brief Implementation of the Spark Engine Editor UI system
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorUI.h"
#include "EditorTheme.h"
#include "../AssetBrowser/AssetDatabase.h"
#include "../Communication/EngineInterface.h"
#include "../SceneSystem/SceneManager.h"
#include <imgui.h>
#include <iostream>

namespace SparkEditor {

EditorUI::EditorUI()
{
    std::cout << "Spark Engine Editor UI initialized\n";
}

EditorUI::~EditorUI()
{
    std::cout << "EditorUI destructor called\n";
    Shutdown();
}

bool EditorUI::Initialize()
{
    std::cout << "EditorUI::Initialize()\n";
    
    if (m_isInitialized) {
        return true;
    }
    
    // Setup Dear ImGui context
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Professional settings
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigDragClickToInputText = true;
    
    // Apply Spark Engine theme
    EditorTheme::ApplyTheme("Unity Pro");
    EditorTheme::ApplyProfessionalEnhancements();
    m_currentTheme = "Unity Pro";
    
    // Create all editor panels
    CreatePanels();
    
    // Initialize subsystem references
    SetupSubsystemIntegration();
    
    m_isInitialized = true;
    std::cout << "Spark Engine Editor UI initialized successfully\n";
    return true;
}

void EditorUI::Update(float deltaTime)
{
    if (!m_isInitialized) {
        return;
    }
    
    // Update UI metrics
    m_uiMetrics.lastFrameTime = deltaTime;
    
    // Handle UI events and shortcuts
    HandleUIEvents();
    
    // Update subsystem integration
    UpdateSubsystemIntegration(deltaTime);
    
    // Update all panels
    UpdatePanels(deltaTime);
}

void EditorUI::Render()
{
    if (!m_isInitialized) {
        return;
    }
    
    // Render main menu bar
    RenderSparkMenuBar();
    
    // Render Spark toolbar
    RenderSparkToolbar();
    
    // Render all panels with Spark styling
    RenderSparkPanels();
    
    // Render status bar
    RenderSparkStatusBar();
    
    // Show ImGui demo window if enabled
    if (m_showDemoWindow) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }
    
    // Show theme editor if enabled
    static bool showThemeEditor = false;
    if (showThemeEditor) {
        ThemeCustomizer::ShowThemeEditor();
    }
    
    // Check for theme editor shortcut
    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_T)) {
        showThemeEditor = !showThemeEditor;
    }
    
    // Render modal dialog if active
    if (m_currentDialog.isOpen) {
        RenderSparkDialog();
    }
}

void EditorUI::Shutdown()
{
    std::cout << "EditorUI::Shutdown()\n";
    
    if (!m_isInitialized) {
        return;
    }
    
    m_isInitialized = false;
}

bool EditorUI::IsPanelVisible(const std::string& panelName) const
{
    auto it = m_panelVisibility.find(panelName);
    if (it != m_panelVisibility.end()) {
        return it->second;
    }
    return false;
}

void EditorUI::SetPanelVisible(const std::string& panelName, bool visible)
{
    m_panelVisibility[panelName] = visible;
    std::cout << "Panel '" << panelName << "' visibility set to " << (visible ? "true" : "false") << "\n";
}

bool EditorUI::SaveLayout(const std::string& layoutName)
{
    std::cout << "Saving layout: " << layoutName << "\n";
    // TODO: Implement layout saving with ImGui's .ini system
    return true;
}

bool EditorUI::LoadLayout(const std::string& layoutName)
{
    std::cout << "Loading layout: " << layoutName << "\n";
    // TODO: Implement layout loading
    return true;
}

void EditorUI::ResetToDefaultLayout()
{
    std::cout << "Resetting to default Spark Engine layout\n";
    SetupDefaultDocking();
}

std::vector<std::string> EditorUI::GetAvailableLayouts() const
{
    return {"Spark Default", "Engine Debug", "Performance", "Asset Creation", "Lighting Design", "Animation"};
}

void EditorUI::ApplyTheme(const std::string& themeName)
{
    m_currentTheme = themeName;
    std::cout << "Applying Spark Engine theme: " << themeName << "\n";
    
    EditorTheme::ApplyTheme(themeName);
    EditorTheme::ApplyProfessionalEnhancements();
}

void EditorUI::ShowContextMenu(const std::unordered_map<std::string, std::function<void()>>& menuItems)
{
    if (ImGui::BeginPopupContextWindow()) {
        for (const auto& item : menuItems) {
            if (ImGui::MenuItem(item.first.c_str())) {
                if (item.second) {
                    item.second();
                }
            }
        }
        ImGui::EndPopup();
    }
}

void EditorUI::ShowModalDialog(const std::string& title, 
                              std::function<void()> content,
                              const std::unordered_map<std::string, std::function<void()>>& buttons)
{
    m_currentDialog.title = title;
    m_currentDialog.content = content;
    m_currentDialog.buttons = buttons;
    m_currentDialog.isOpen = true;
}

void EditorUI::CreatePanels()
{
    std::cout << "Creating Spark Engine editor panels\n";
    
    // Initialize panel visibility with Spark-specific defaults
    m_panelVisibility["SceneGraph"] = true;          // Spark's scene hierarchy
    m_panelVisibility["EntityInspector"] = true;     // Entity component inspector
    m_panelVisibility["AssetExplorer"] = true;       // Asset browser with real-time monitoring
    m_panelVisibility["Viewport3D"] = true;          // 3D scene viewport
    m_panelVisibility["GamePreview"] = true;         // Game runtime preview
    m_panelVisibility["EngineConsole"] = true;       // Engine debug console
    m_panelVisibility["PerformanceProfiler"] = false; // Performance monitoring
    m_panelVisibility["LightingWorkspace"] = false;  // Advanced lighting tools
    m_panelVisibility["ShaderEditor"] = false;       // Shader development
    m_panelVisibility["PhysicsDebugger"] = false;    // Physics system debug
    m_panelVisibility["AudioMixer"] = false;         // Audio system controls
    m_panelVisibility["EngineSettings"] = false;     // Engine configuration
    
    std::cout << "Spark Engine editor panels created successfully\n";
}

void EditorUI::SetupSubsystemIntegration()
{
    // Initialize connections to engine subsystems
    m_engineConnected = false; // Will be set by EngineInterface
    m_assetDatabaseSize = 0;
    m_sceneObjectCount = 0;
    
    std::cout << "Subsystem integration setup complete\n";
}

void EditorUI::UpdateSubsystemIntegration(float deltaTime)
{
    // Update integration with engine subsystems
    // This would normally be updated by the actual subsystem references
    static float updateTimer = 0.0f;
    updateTimer += deltaTime;
    
    // Update every 0.5 seconds to avoid spam
    if (updateTimer >= 0.5f) {
        // TODO: Update from actual subsystems when available
        updateTimer = 0.0f;
    }
}

void EditorUI::RenderSparkMenuBar()
{
    if (ImGui::BeginMainMenuBar()) {
        // Spark Engine Menu
        if (ImGui::BeginMenu("Spark Engine")) {
            if (ImGui::MenuItem("New Project", "Ctrl+Shift+N")) {
                std::cout << "New Spark Engine project requested\n";
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O")) {
                std::cout << "Open Spark Engine project requested\n";
            }
            if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) {
                std::cout << "Save Spark Engine project requested\n";
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Build & Package", "F7")) {
                std::cout << "Build and package requested\n";
            }
            if (ImGui::MenuItem("Deploy", "Shift+F7")) {
                std::cout << "Deploy requested\n";
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Engine Settings")) {
                m_panelVisibility["EngineSettings"] = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit Editor", "Alt+F4")) {
                std::cout << "Exit Spark Engine Editor requested\n";
            }
            ImGui::EndMenu();
        }
        
        // Scene Menu
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                std::cout << "New scene requested\n";
            }
            if (ImGui::MenuItem("Load Scene", "Ctrl+O")) {
                std::cout << "Load scene requested\n";
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                std::cout << "Save scene requested\n";
            }
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Alt+S")) {
                std::cout << "Save scene as requested\n";
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Create Entity")) {
                if (ImGui::MenuItem("Empty Entity")) {
                    std::cout << "Create empty entity requested\n";
                }
                if (ImGui::MenuItem("Mesh Entity")) {
                    std::cout << "Create mesh entity requested\n";
                }
                if (ImGui::MenuItem("Light Entity")) {
                    std::cout << "Create light entity requested\n";
                }
                if (ImGui::MenuItem("Camera Entity")) {
                    std::cout << "Create camera entity requested\n";
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        
        // Assets Menu
        if (ImGui::BeginMenu("Assets")) {
            if (ImGui::BeginMenu("Import")) {
                if (ImGui::MenuItem("Model (.fbx, .obj)")) {
                    std::cout << "Import model requested\n";
                }
                if (ImGui::MenuItem("Texture")) {
                    std::cout << "Import texture requested\n";
                }
                if (ImGui::MenuItem("Audio")) {
                    std::cout << "Import audio requested\n";
                }
                if (ImGui::MenuItem("Shader")) {
                    std::cout << "Import shader requested\n";
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Refresh Asset Database", "F5")) {
                std::cout << "Refresh asset database requested\n";
            }
            if (ImGui::MenuItem("Reimport All Assets")) {
                std::cout << "Reimport all assets requested\n";
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Asset Pipeline Settings")) {
                std::cout << "Asset pipeline settings requested\n";
            }
            ImGui::EndMenu();
        }
        
        // Tools Menu
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::BeginMenu("Lighting")) {
                ImGui::MenuItem("Lighting Workspace", nullptr, &m_panelVisibility["LightingWorkspace"]);
                if (ImGui::MenuItem("Bake Lightmaps")) {
                    std::cout << "Bake lightmaps requested\n";
                }
                if (ImGui::MenuItem("Generate Light Probes")) {
                    std::cout << "Generate light probes requested\n";
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Shaders")) {
                ImGui::MenuItem("Shader Editor", nullptr, &m_panelVisibility["ShaderEditor"]);
                if (ImGui::MenuItem("Compile All Shaders")) {
                    std::cout << "Compile all shaders requested\n";
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Physics")) {
                ImGui::MenuItem("Physics Debugger", nullptr, &m_panelVisibility["PhysicsDebugger"]);
                if (ImGui::MenuItem("Physics Settings")) {
                    std::cout << "Physics settings requested\n";
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Audio")) {
                ImGui::MenuItem("Audio Mixer", nullptr, &m_panelVisibility["AudioMixer"]);
                if (ImGui::MenuItem("Audio Settings")) {
                    std::cout << "Audio settings requested\n";
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            ImGui::MenuItem("Performance Profiler", nullptr, &m_panelVisibility["PerformanceProfiler"]);
            ImGui::EndMenu();
        }
        
        // Windows Menu
        if (ImGui::BeginMenu("Windows")) {
            if (ImGui::BeginMenu("Core")) {
                ImGui::MenuItem("Scene Graph", nullptr, &m_panelVisibility["SceneGraph"]);
                ImGui::MenuItem("Entity Inspector", nullptr, &m_panelVisibility["EntityInspector"]);
                ImGui::MenuItem("Asset Explorer", nullptr, &m_panelVisibility["AssetExplorer"]);
                ImGui::MenuItem("Engine Console", nullptr, &m_panelVisibility["EngineConsole"]);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Viewport")) {
                ImGui::MenuItem("3D Viewport", nullptr, &m_panelVisibility["Viewport3D"]);
                ImGui::MenuItem("Game Preview", nullptr, &m_panelVisibility["GamePreview"]);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Advanced")) {
                ImGui::MenuItem("Lighting Workspace", nullptr, &m_panelVisibility["LightingWorkspace"]);
                ImGui::MenuItem("Shader Editor", nullptr, &m_panelVisibility["ShaderEditor"]);
                ImGui::MenuItem("Physics Debugger", nullptr, &m_panelVisibility["PhysicsDebugger"]);
                ImGui::MenuItem("Audio Mixer", nullptr, &m_panelVisibility["AudioMixer"]);
                ImGui::MenuItem("Performance Profiler", nullptr, &m_panelVisibility["PerformanceProfiler"]);
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Layouts")) {
                auto layouts = GetAvailableLayouts();
                for (const auto& layout : layouts) {
                    if (ImGui::MenuItem(layout.c_str())) {
                        LoadLayout(layout);
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Reset to Default")) {
                    ResetToDefaultLayout();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Themes")) {
                auto availableThemes = EditorTheme::GetAvailableThemes();
                for (const auto& themeName : availableThemes) {
                    bool isSelected = (m_currentTheme == themeName);
                    if (ImGui::MenuItem(themeName.c_str(), nullptr, isSelected)) {
                        ApplyTheme(themeName);
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Theme Editor...", "Ctrl+Shift+T")) {
                    ThemeCustomizer::ShowThemeEditor();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", nullptr, &m_showDemoWindow);
            ImGui::EndMenu();
        }
        
        // Help Menu
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About Spark Engine")) {
                ShowModalDialog("About Spark Engine Editor", 
                    []() {
                        ImGui::Text("? Spark Engine Editor v1.0.0");
                        ImGui::Separator();
                        ImGui::Text("Advanced Game Development Environment");
                        ImGui::Text("Built with DirectX 11 and Dear ImGui");
                        ImGui::Separator();
                        ImGui::Text("Features:");
                        ImGui::BulletText("Real-time asset monitoring");
                        ImGui::BulletText("Advanced lighting tools");
                        ImGui::BulletText("Live engine integration");
                        ImGui::BulletText("Performance profiling");
                        ImGui::BulletText("Advanced shader development");
                        ImGui::Separator();
                        ImGui::Text("© 2025 Spark Engine Team");
                        ImGui::Text("Built for game developers by game developers");
                    },
                    {{"OK", nullptr}});
            }
            if (ImGui::MenuItem("Documentation")) {
                std::cout << "Documentation requested\n";
            }
            if (ImGui::MenuItem("Report Issue")) {
                std::cout << "Report issue requested\n";
            }
            if (ImGui::MenuItem("Community Forum")) {
                std::cout << "Community forum requested\n";
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void EditorUI::RenderSparkToolbar()
{
    ImGuiWindowFlags toolbar_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoSavedSettings;
    
    ImGui::Begin("##SparkToolbar", nullptr, toolbar_flags);
    
    // Engine control section
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.7f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
    if (ImGui::Button("? Start Engine", ImVec2(100, 0))) {
        std::cout << "Start Spark Engine requested\n";
    }
    ImGui::PopStyleColor(2);
    
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.6f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.7f, 0.2f, 1.0f));
    if (ImGui::Button("? Pause", ImVec2(80, 0))) {
        std::cout << "Pause engine requested\n";
    }
    ImGui::PopStyleColor(2);
    
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    if (ImGui::Button("? Stop", ImVec2(80, 0))) {
        std::cout << "Stop engine requested\n";
    }
    ImGui::PopStyleColor(2);
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Transformation tools
    if (ImGui::Button("?? Rotate", ImVec2(80, 0))) {
        std::cout << "Rotate tool selected\n";
    }
    ImGui::SameLine();
    
    if (ImGui::Button("?? Scale", ImVec2(80, 0))) {
        std::cout << "Scale tool selected\n";
    }
    ImGui::SameLine();
    
    if (ImGui::Button("?? Move", ImVec2(80, 0))) {
        std::cout << "Move tool selected\n";
    }
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Build tools
    if (ImGui::Button("?? Build", ImVec2(80, 0))) {
        std::cout << "Build project requested\n";
    }
    ImGui::SameLine();
    
    if (ImGui::Button("?? Deploy", ImVec2(80, 0))) {
        std::cout << "Deploy project requested\n";
    }
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Performance monitoring
    if (m_engineConnected) {
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "? Engine Connected");
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "? Engine Disconnected");
    }
    
    ImGui::End();
}

void EditorUI::RenderSparkPanels()
{
    // Scene Graph Panel (Spark's hierarchy)
    if (m_panelVisibility["SceneGraph"]) {
        ImGui::Begin("? Scene Graph", &m_panelVisibility["SceneGraph"]);
        
        ImGui::Text("Scene Entities (%d)", m_sceneObjectCount);
        ImGui::Separator();
        
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.0f);
        
        if (ImGui::TreeNode("?? MainScene")) {
            if (ImGui::TreeNode("?? MainCamera")) {
                ImGui::Text("  Transform");
                ImGui::Text("  Camera Component");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("?? DirectionalLight")) {
                ImGui::Text("  Transform");
                ImGui::Text("  Light Component");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("?? Environment")) {
                if (ImGui::TreeNode("??? Terrain")) {
                    ImGui::Text("  Mesh Renderer");
                    ImGui::Text("  Terrain Component");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("?? Vegetation")) {
                    ImGui::Text("  Instanced Renderer");
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("?? Player")) {
                ImGui::Text("  Transform");
                ImGui::Text("  Player Controller");
                ImGui::Text("  Physics Body");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        
        ImGui::PopStyleVar();
        ImGui::End();
    }
    
    // Entity Inspector Panel
    if (m_panelVisibility["EntityInspector"]) {
        ImGui::Begin("?? Entity Inspector", &m_panelVisibility["EntityInspector"]);
        
        ImGui::Text("?? Player Entity");
        ImGui::Separator();
        
        // Transform component
        if (ImGui::CollapsingHeader("?? Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Position");
            ImGui::SameLine(100);
            static float pos[3] = {0.0f, 0.0f, 0.0f};
            ImGui::DragFloat3("##pos", pos, 0.1f);
            
            ImGui::Text("Rotation");
            ImGui::SameLine(100);
            static float rot[3] = {0.0f, 0.0f, 0.0f};
            ImGui::DragFloat3("##rot", rot, 1.0f);
            
            ImGui::Text("Scale");
            ImGui::SameLine(100);
            static float scale[3] = {1.0f, 1.0f, 1.0f};
            ImGui::DragFloat3("##scale", scale, 0.1f);
        }
        
        // Player Controller component
        if (ImGui::CollapsingHeader("?? Player Controller")) {
            static float speed = 5.0f;
            ImGui::SliderFloat("Move Speed", &speed, 0.1f, 20.0f);
            
            static float jumpHeight = 2.0f;
            ImGui::SliderFloat("Jump Height", &jumpHeight, 0.1f, 10.0f);
            
            static bool godMode = false;
            ImGui::Checkbox("God Mode", &godMode);
        }
        
        // Physics Body component
        if (ImGui::CollapsingHeader("?? Physics Body")) {
            static float mass = 1.0f;
            ImGui::SliderFloat("Mass", &mass, 0.1f, 100.0f);
            
            static bool useGravity = true;
            ImGui::Checkbox("Use Gravity", &useGravity);
            
            static float friction = 0.5f;
            ImGui::SliderFloat("Friction", &friction, 0.0f, 1.0f);
        }
        
        // Add Component button
        ImGui::Separator();
        if (ImGui::Button("? Add Component", ImVec2(-1, 0))) {
            std::cout << "Add component to entity requested\n";
        }
        
        ImGui::End();
    }
    
    // Asset Explorer Panel
    if (m_panelVisibility["AssetExplorer"]) {
        ImGui::Begin("?? Asset Explorer", &m_panelVisibility["AssetExplorer"]);
        
        // Asset database info
        ImGui::Text("?? Asset Database (%d assets)", m_assetDatabaseSize);
        ImGui::SameLine();
        if (ImGui::Button("?? Refresh")) {
            std::cout << "Refresh asset database requested\n";
        }
        ImGui::Separator();
        
        // Folder structure
        ImGui::Columns(2, "AssetColumns");
        ImGui::SetColumnWidth(0, 200);
        
        // Left side - folder tree
        ImGui::Text("?? Assets");
        if (ImGui::TreeNode("?? Textures")) {
            ImGui::Text("  ??? Character_Diffuse.png");
            ImGui::Text("  ??? Environment_Normal.png");
            ImGui::Text("  ??? UI_Icons.png");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("?? Models")) {
            ImGui::Text("  ?? Character.fbx");
            ImGui::Text("  ??? Building.obj");
            ImGui::Text("  ?? Tree.fbx");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("?? Audio")) {
            ImGui::Text("  ?? Background.ogg");
            ImGui::Text("  ?? Footsteps.wav");
            ImGui::Text("  ?? Explosion.wav");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("?? Shaders")) {
            ImGui::Text("  ?? PBR_Standard.hlsl");
            ImGui::Text("  ?? Particle.hlsl");
            ImGui::Text("  ?? Water.hlsl");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("?? Scenes")) {
            ImGui::Text("  ?? MainMenu.scene");
            ImGui::Text("  ?? Level1.scene");
            ImGui::Text("  ?? TestLevel.scene");
            ImGui::TreePop();
        }
        
        ImGui::NextColumn();
        
        // Right side - asset preview
        ImGui::Text("?? Asset Preview");
        ImGui::Separator();
        
        // Grid view of assets
        for (int i = 0; i < 6; i++) {
            if (i % 3 != 0) ImGui::SameLine();
            ImGui::Button("?? Asset", ImVec2(80, 80));
        }
        
        ImGui::Columns(1);
        ImGui::End();
    }
    
    // 3D Viewport Panel
    if (m_panelVisibility["Viewport3D"]) {
        ImGui::Begin("?? 3D Viewport", &m_panelVisibility["Viewport3D"]);
        
        // Viewport controls
        if (ImGui::Button("?? Perspective")) {
            std::cout << "Perspective viewport\n";
        }
        ImGui::SameLine();
        if (ImGui::Button("?? Orthographic")) {
            std::cout << "Orthographic viewport\n";
        }
        ImGui::SameLine();
        if (ImGui::Button("?? Focus Selected")) {
            std::cout << "Focus on selected object\n";
        }
        
        ImGui::Separator();
        
        // Viewport rendering area
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        
        ImGui::GetForegroundDrawList()->AddRectFilled(vMin, vMax, IM_COL32(32, 32, 32, 255));
        ImGui::GetForegroundDrawList()->AddText(ImVec2(vMin.x + 10, vMin.y + 10), IM_COL32_WHITE, "? Spark Engine 3D Viewport");
        
        ImGui::End();
    }
    
    // Game Preview Panel
    if (m_panelVisibility["GamePreview"]) {
        ImGui::Begin("?? Game Preview", &m_panelVisibility["GamePreview"]);
        
        // Game preview controls
        static int resolution = 0;
        const char* resolutions[] = { "1920x1080", "1280x720", "1024x768", "800x600" };
        ImGui::Combo("Resolution", &resolution, resolutions, IM_ARRAYSIZE(resolutions));
        
        ImGui::SameLine();
        static bool fullscreen = false;
        ImGui::Checkbox("Fullscreen Preview", &fullscreen);
        
        ImGui::Separator();
        
        // Game rendering area
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        
        ImGui::GetForegroundDrawList()->AddRectFilled(vMin, vMax, IM_COL32(16, 16, 16, 255));
        ImGui::GetForegroundDrawList()->AddText(ImVec2(vMin.x + 10, vMin.y + 10), IM_COL32_WHITE, "?? Spark Engine Game Preview");
        
        ImGui::End();
    }
    
    // Engine Console Panel
    if (m_panelVisibility["EngineConsole"]) {
        ImGui::Begin("??? Engine Console", &m_panelVisibility["EngineConsole"]);
        
        // Console controls
        if (ImGui::Button("??? Clear")) {
            std::cout << "Engine console cleared\n";
        }
        ImGui::SameLine();
        static bool autoscroll = true;
        ImGui::Checkbox("?? Auto Scroll", &autoscroll);
        ImGui::SameLine();
        static bool timestamps = true;
        ImGui::Checkbox("? Timestamps", &timestamps);
        
        ImGui::Separator();
        
        // Engine log messages
        ImGui::BeginChild("ConsoleScrolling");
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "? [Spark] Engine initialized successfully");
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "? [Asset] Real-time monitoring active");
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "?? [Warning] Shader compilation took 150ms");
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "? [Error] Failed to load texture: missing.png");
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "? [Spark] Scene loaded: MainLevel");
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "?? [Debug] Physics simulation started");
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "? [Success] Lightmaps baked successfully");
        ImGui::EndChild();
        
        ImGui::End();
    }
    
    // Performance Profiler Panel
    if (m_panelVisibility["PerformanceProfiler"]) {
        ImGui::Begin("?? Performance Profiler", &m_panelVisibility["PerformanceProfiler"]);
        
        ImGui::Text("?? Spark Engine Performance Monitor");
        ImGui::Separator();
        
        // Performance metrics
        ImGui::Text("FPS: %.1f", 1.0f / m_uiMetrics.lastFrameTime);
        ImGui::SameLine(150);
        ImGui::Text("Frame Time: %.3f ms", m_uiMetrics.lastFrameTime * 1000.0f);
        
        static float cpuUsage = 25.0f;
        ImGui::ProgressBar(cpuUsage / 100.0f, ImVec2(-1, 0), "CPU Usage");
        
        static float gpuUsage = 60.0f;
        ImGui::ProgressBar(gpuUsage / 100.0f, ImVec2(-1, 0), "GPU Usage");
        
        static float memoryUsage = 512.0f;
        ImGui::Text("Memory: %.0f MB", memoryUsage);
        
        ImGui::Separator();
        ImGui::Text("Rendering Stats:");
        ImGui::BulletText("Draw Calls: 145");
        ImGui::BulletText("Triangles: 125,437");
        ImGui::BulletText("Vertices: 89,234");
        
        ImGui::End();
    }
    
    // Lighting Workspace Panel
    if (m_panelVisibility["LightingWorkspace"]) {
        ImGui::Begin("?? Lighting Workspace", &m_panelVisibility["LightingWorkspace"]);
        
        ImGui::Text("? Spark Engine Advanced Lighting");
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("?? Environment", ImGuiTreeNodeFlags_DefaultOpen)) {
            static float skyboxRotation = 0.0f;
            ImGui::SliderFloat("Skybox Rotation", &skyboxRotation, 0.0f, 360.0f);
            
            static float ambientIntensity = 1.0f;
            ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 3.0f);
            
            static float fogDensity = 0.1f;
            ImGui::SliderFloat("Fog Density", &fogDensity, 0.0f, 1.0f);
        }
        
        if (ImGui::CollapsingHeader("? Real-time Lighting")) {
            static bool enableGI = true;
            ImGui::Checkbox("Global Illumination", &enableGI);
            
            static bool enableSSAO = true;
            ImGui::Checkbox("Screen Space AO", &enableSSAO);
            
            static float shadowQuality = 1.0f;
            ImGui::SliderFloat("Shadow Quality", &shadowQuality, 0.1f, 2.0f);
        }
        
        if (ImGui::CollapsingHeader("?? Time of Day")) {
            static float timeOfDay = 12.0f;
            ImGui::SliderFloat("Time (Hours)", &timeOfDay, 0.0f, 24.0f);
            
            static float sunIntensity = 3.0f;
            ImGui::SliderFloat("Sun Intensity", &sunIntensity, 0.0f, 8.0f);
            
            static float sunTemperature = 5500.0f;
            ImGui::SliderFloat("Sun Temperature (K)", &sunTemperature, 3000.0f, 8000.0f);
        }
        
        ImGui::Separator();
        if (ImGui::Button("?? Bake Lightmaps", ImVec2(-1, 0))) {
            std::cout << "Lightmap baking requested\n";
        }
        
        ImGui::End();
    }
}

void EditorUI::RenderSparkStatusBar()
{
    ImGuiWindowFlags status_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoSavedSettings;
    
    ImGui::Begin("##SparkStatusBar", nullptr, status_flags);
    
    // Left side - engine status
    if (m_engineConnected) {
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "? Spark Engine Connected");
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "?? Spark Engine Standalone Mode");
    }
    
    ImGui::SameLine(300);
    ImGui::Text("| ?? FPS: %.1f", 1.0f / m_uiMetrics.lastFrameTime);
    ImGui::SameLine(400);
    ImGui::Text("| ?? %.3f ms", m_uiMetrics.lastFrameTime * 1000.0f);
    
    // Right side - theme and asset info
    ImGui::SameLine(ImGui::GetWindowWidth() - 300);
    ImGui::Text("?? Assets: %d", m_assetDatabaseSize);
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Text("?? %s", m_currentTheme.c_str());
    
    ImGui::End();
}

void EditorUI::RenderSparkDialog()
{
    ImGui::OpenPopup(m_currentDialog.title.c_str());
    
    // Center the dialog
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal(m_currentDialog.title.c_str(), &m_currentDialog.isOpen, 
                              ImGuiWindowFlags_AlwaysAutoResize)) {
        if (m_currentDialog.content) {
            m_currentDialog.content();
        }
        
        ImGui::Separator();
        
        // Center the buttons
        float button_width = 120.0f;
        float button_spacing = 10.0f;
        float total_width = m_currentDialog.buttons.size() * button_width + 
                           (m_currentDialog.buttons.size() - 1) * button_spacing;
        float offset = (ImGui::GetWindowWidth() - total_width) * 0.5f;
        
        ImGui::SetCursorPosX(offset);
        
        bool first = true;
        for (const auto& button : m_currentDialog.buttons) {
            if (!first) {
                ImGui::SameLine();
            }
            first = false;
            
            if (ImGui::Button(button.first.c_str(), ImVec2(button_width, 0))) {
                if (button.second) {
                    button.second();
                }
                m_currentDialog.isOpen = false;
            }
        }
        
        ImGui::EndPopup();
    }
}

void EditorUI::SetupDefaultDocking()
{
    std::cout << "Setting up Spark Engine default layout\n";
}

void EditorUI::HandleUIEvents()
{
    ImGuiIO& io = ImGui::GetIO();
    
    // Spark Engine specific shortcuts
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_N)) {
        std::cout << "Ctrl+Shift+N pressed - New Spark Project\n";
    }
    
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_O)) {
        std::cout << "Ctrl+Shift+O pressed - Open Spark Project\n";
    }
    
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_S)) {
        std::cout << "Ctrl+Shift+S pressed - Save Spark Project\n";
    }
    
    // Standard scene shortcuts
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N)) {
        std::cout << "Ctrl+N pressed - New Scene\n";
    }
    
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O)) {
        std::cout << "Ctrl+O pressed - Load Scene\n";
    }
    
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
        std::cout << "Ctrl+S pressed - Save Scene\n";
    }
    
    // Build shortcuts
    if (ImGui::IsKeyPressed(ImGuiKey_F7)) {
        std::cout << "F7 pressed - Build Project\n";
    }
    
    if (io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_F7)) {
        std::cout << "Shift+F7 pressed - Deploy Project\n";
    }
    
    // Refresh assets
    if (ImGui::IsKeyPressed(ImGuiKey_F5)) {
        std::cout << "F5 pressed - Refresh Asset Database\n";
    }
}

void EditorUI::UpdatePanels(float deltaTime)
{
    // Update Spark Engine specific panel data
    (void)deltaTime; // Suppress unused parameter warning
}

std::string EditorUI::ShowFileDialog(const std::string& title, 
                                    const char* filters, 
                                    const std::string& defaultPath, 
                                    bool save)
{
    std::cout << "Spark Engine file dialog requested: " << title << "\n";
    // TODO: Implement native file dialog with Spark Engine integration
    return "";
}

} // namespace SparkEditor