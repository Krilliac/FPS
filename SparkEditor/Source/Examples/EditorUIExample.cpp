/**
 * @file EditorUIExample.cpp
 * @brief Example implementation showing how to integrate all the new editor systems
 * @author Spark Engine Team
 * @date 2025
 */

#include "Core/EditorUI.h"
#include "Core/EditorLogger.h"
#include "Core/EditorLayoutManager.h"
#include "Core/EditorCrashHandler.h"
#include "Panels/ConsolePanel.h"

namespace SparkEditor {

// Example implementation of enhanced EditorUI
class EnhancedEditorUI : public EditorUI {
public:
    bool InitializeEnhanced() {
        // Configure editor
        EditorConfig config;
        config.projectPath = "ExampleProject";
        config.defaultLayout = "Spark Default";
        config.enableLogging = true;
        config.enableCrashHandler = true;
        config.enableExternalConsole = true;
        config.autoSaveInterval = 30.0f;
        
        // Initialize with enhanced features
        if (!Initialize(config)) {
            return false;
        }
        
        // Setup custom commands
        RegisterCommand("test_logging", 
            [this](const std::vector<std::string>& args) -> std::string {
                TestLoggingLevels();
                return "Logging test completed - check console for different log levels";
            },
            "Test all logging levels and categories"
        );
        
        RegisterCommand("test_crash", 
            [this](const std::vector<std::string>& args) -> std::string {
                if (!args.empty() && args[0] == "confirm") {
                    GetCrashHandler()->TestCrashHandler();
                    return "This should not appear";
                } else {
                    return "Use 'test_crash confirm' to actually trigger a test crash";
                }
            },
            "Test crash handler (use with 'confirm')"
        );
        
        RegisterCommand("layout", 
            [this](const std::vector<std::string>& args) -> std::string {
                if (args.empty()) {
                    auto layouts = GetAvailableLayouts();
                    std::string result = "Available layouts:\n";
                    for (const auto& layout : layouts) {
                        result += "  - " + layout + "\n";
                    }
                    return result;
                } else if (args[0] == "save" && args.size() > 1) {
                    if (SaveLayout(args[1], "Saved via command")) {
                        return "Layout saved: " + args[1];
                    } else {
                        return "Failed to save layout: " + args[1];
                    }
                } else if (args[0] == "load" && args.size() > 1) {
                    if (LoadLayout(args[1])) {
                        return "Layout loaded: " + args[1];
                    } else {
                        return "Failed to load layout: " + args[1];
                    }
                } else {
                    return "Usage: layout [save <name>] [load <name>]";
                }
            },
            "Manage editor layouts"
        );
        
        RegisterCommand("performance", 
            [this](const std::vector<std::string>& args) -> std::string {
                auto stats = GetStats();
                std::stringstream ss;
                ss << "Editor Performance Stats:\n";
                ss << "  Frame Time: " << std::fixed << std::setprecision(2) << stats.frameTime << "ms\n";
                ss << "  Average Frame Time: " << stats.averageFrameTime << "ms\n";
                ss << "  Draw Calls: " << stats.drawCalls << "\n";
                ss << "  Memory Usage: " << (stats.memoryUsage / 1024 / 1024) << "MB\n";
                ss << "  Visible Panels: " << stats.visiblePanels << "/" << stats.totalPanels << "\n";
                ss << "  Engine Connected: " << (IsEngineConnected() ? "Yes" : "No") << "\n";
                return ss.str();
            },
            "Show editor performance statistics"
        );
        
        return true;
    }
    
    void TestLoggingLevels() {
        auto* logger = GetLogger();
        if (!logger) return;
        
        // Test all log levels and categories
        EDITOR_LOG_TRACE(LogCategory::GENERAL, "This is a TRACE level message for testing");
        EDITOR_LOG_DEBUG(LogCategory::ASSET, "This is a DEBUG level message for asset system");
        EDITOR_LOG_INFO(LogCategory::RENDERING, "This is an INFO level message for rendering");
        EDITOR_LOG_WARNING(LogCategory::ENGINE, "This is a WARNING level message for engine communication");
        EDITOR_LOG_ERROR(LogCategory::UI, "This is an ERROR level message for UI system");
        EDITOR_LOG_CRITICAL(LogCategory::PHYSICS, "This is a CRITICAL level message for physics");
        
        // Test formatted logging
        EDITOR_LOG_INFO_F(LogCategory::PROFILING, "Performance test: %d FPS, %.2fms frame time", 60, 16.67f);
        
        // Test metadata logging
        logger->LogWithMetadata(LogLevel::INFO, LogCategory::SCRIPTING, 
            "Script execution completed",
            {{"scriptName", "TestScript.cs"}, 
             {"executionTime", "125ms"}, 
             {"memoryUsed", "2.5MB"}});
        
        // Record operations for crash handler
        EDITOR_RECORD_OPERATION("Testing logging system");
        EDITOR_SET_STATE("Running logging level tests");
        
        logger->Log(LogLevel::INFO, LogCategory::GENERAL, 
                   "Logging test completed - all levels and categories tested");
    }
    
    void DemoLayoutSwitching() {
        auto* layoutManager = GetLayoutManager();
        if (!layoutManager) return;
        
        // Demonstrate layout switching
        ShowNotification("Switching to Engine Debug layout", "info", 3.0f);
        LoadLayout("Engine Debug");
        
        // Schedule return to default after 10 seconds (in a real app)
        // std::thread([this]() {
        //     std::this_thread::sleep_for(std::chrono::seconds(10));
        //     LoadLayout("Spark Default");
        //     ShowNotification("Returned to Spark Default layout", "info", 3.0f);
        // }).detach();
    }
    
    void DemoRecoverySystem() {
        if (HasRecoveryData()) {
            if (ShowRecoveryDialog()) {
                ShowNotification("Editor state recovered successfully", "info", 5.0f);
                EDITOR_LOG_INFO(LogCategory::GENERAL, "Editor recovery completed");
            } else {
                GetCrashHandler()->ClearRecoveryData();
                ShowNotification("Starting fresh - recovery data cleared", "info", 3.0f);
            }
        }
    }
    
    void SimulateEditorWorkflow() {
        // Simulate typical editor operations
        EDITOR_RECORD_OPERATION("Opened project: ExampleProject");
        EDITOR_SET_STATE("Project loaded");
        
        UpdateAssetDatabaseInfo(1234, 256 * 1024 * 1024); // 1234 assets, 256MB
        UpdateSceneInfo(567, 3); // 567 objects, 3 selected
        
        EDITOR_LOG_INFO(LogCategory::ASSET, "Asset database loaded with 1234 assets");
        EDITOR_LOG_INFO(LogCategory::GENERAL, "Scene loaded with 567 objects");
        
        // Simulate engine connection
        SetEngineConnected(true);
        EDITOR_LOG_INFO(LogCategory::ENGINE, "Connected to Spark Engine");
        
        ShowNotification("Editor ready - all systems initialized", "info", 4.0f);
    }
};

} // namespace SparkEditor

// Example usage function
void RunEnhancedEditorExample() {
    using namespace SparkEditor;
    
    // Create enhanced editor UI
    EnhancedEditorUI editorUI;
    
    // Initialize with all enhanced features
    if (!editorUI.InitializeEnhanced()) {
        // Handle initialization failure
        return;
    }
    
    // Demonstrate recovery system
    editorUI.DemoRecoverySystem();
    
    // Simulate editor workflow
    editorUI.SimulateEditorWorkflow();
    
    // Main editor loop would go here
    // while (running) {
    //     float deltaTime = CalculateDeltaTime();
    //     editorUI.Update(deltaTime);
    //     editorUI.Render();
    // }
    
    // Cleanup
    editorUI.Shutdown();
}

// Console command registration example
void RegisterEditorCommands(EnhancedEditorUI* editorUI) {
    // Asset management commands
    editorUI->RegisterCommand("asset_reload", 
        [](const std::vector<std::string>& args) -> std::string {
            if (args.empty()) {
                return "Usage: asset_reload <asset_path>";
            }
            // Reload specific asset
            EDITOR_RECORD_OPERATION("Reloading asset: " + args[0]);
            EDITOR_LOG_INFO(LogCategory::ASSET, "Reloading asset: " + args[0]);
            return "Asset reloaded: " + args[0];
        },
        "Reload a specific asset from disk"
    );
    
    // Scene management commands
    editorUI->RegisterCommand("scene_save", 
        [](const std::vector<std::string>& args) -> std::string {
            std::string sceneName = args.empty() ? "current_scene.scene" : args[0];
            EDITOR_RECORD_OPERATION("Saving scene: " + sceneName);
            EDITOR_LOG_INFO(LogCategory::GENERAL, "Scene saved: " + sceneName);
            return "Scene saved: " + sceneName;
        },
        "Save the current scene"
    );
    
    // Performance commands
    editorUI->RegisterCommand("gc", 
        [](const std::vector<std::string>& args) -> std::string {
            EDITOR_RECORD_OPERATION("Manual garbage collection");
            EDITOR_LOG_INFO(LogCategory::PROFILING, "Manual garbage collection triggered");
            // Trigger garbage collection
            return "Garbage collection completed";
        },
        "Trigger manual garbage collection"
    );
    
    // Engine commands
    editorUI->RegisterCommand("engine_restart", 
        [editorUI](const std::vector<std::string>& args) -> std::string {
            EDITOR_RECORD_OPERATION("Restarting engine connection");
            editorUI->SetEngineConnected(false);
            // Simulate restart
            editorUI->SetEngineConnected(true);
            EDITOR_LOG_INFO(LogCategory::ENGINE, "Engine connection restarted");
            return "Engine connection restarted";
        },
        "Restart the engine connection"
    );
}

// Layout customization example
void CreateCustomLayouts(EditorLayoutManager* layoutManager) {
    // Create a custom "Code Review" layout
    LayoutConfig codeReviewLayout;
    codeReviewLayout.name = "Code Review";
    codeReviewLayout.displayName = "?? Code Review";
    codeReviewLayout.description = "Layout optimized for code review and debugging";
    codeReviewLayout.isBuiltIn = false;
    
    // Console takes up more space for logs
    PanelConfig console;
    console.name = "EngineConsole";
    console.displayName = "??? Engine Console";
    console.dockPosition = DockPosition::Bottom;
    console.size = {1200, 400}; // Taller console
    console.dockRatio = 0.4f;   // 40% of screen height
    console.isVisible = true;
    codeReviewLayout.panels[console.name] = console;
    
    // Asset explorer on left
    PanelConfig assets;
    assets.name = "AssetExplorer";
    assets.displayName = "?? Asset Explorer";
    assets.dockPosition = DockPosition::Left;
    assets.size = {350, 600};
    assets.dockRatio = 0.25f;
    assets.isVisible = true;
    codeReviewLayout.panels[assets.name] = assets;
    
    // Code/text editor in center
    PanelConfig codeEditor;
    codeEditor.name = "CodeEditor";
    codeEditor.displayName = "?? Code Editor";
    codeEditor.dockPosition = DockPosition::Center;
    codeEditor.size = {800, 400};
    codeEditor.isVisible = true;
    codeReviewLayout.panels[codeEditor.name] = codeEditor;
    
    // Inspector on right
    PanelConfig inspector;
    inspector.name = "EntityInspector";
    inspector.displayName = "?? Entity Inspector";
    inspector.dockPosition = DockPosition::Right;
    inspector.size = {300, 600};
    inspector.dockRatio = 0.2f;
    inspector.isVisible = true;
    codeReviewLayout.panels[inspector.name] = inspector;
    
    // Register the custom layout
    layoutManager->SaveCurrentLayout("Code Review", "Custom layout for code review tasks");
}

// Comprehensive logging example
void DemonstrateLoggingFeatures() {
    auto& logger = EditorLogger::GetInstance();
    
    // Initialize logging with all outputs
    logger.Initialize("Logs", true);
    
    // Test all logging categories and levels
    EDITOR_LOG_TRACE(LogCategory::GENERAL, "Application startup trace");
    EDITOR_LOG_DEBUG(LogCategory::ASSET, "Loading asset database");
    EDITOR_LOG_INFO(LogCategory::RENDERING, "Initializing DirectX 11 renderer");
    EDITOR_LOG_WARNING(LogCategory::ENGINE, "Engine version mismatch detected");
    EDITOR_LOG_ERROR(LogCategory::PHYSICS, "Physics world creation failed");
    EDITOR_LOG_CRITICAL(LogCategory::NETWORKING, "Network connection lost");
    
    // Formatted logging
    int assetCount = 1234;
    float loadTime = 2.5f;
    EDITOR_LOG_INFO_F(LogCategory::ASSET, "Loaded %d assets in %.2f seconds", assetCount, loadTime);
    
    // Metadata logging for structured data
    logger.LogWithMetadata(LogLevel::INFO, LogCategory::PROFILING,
        "Frame performance metrics",
        {
            {"frameTime", "16.67ms"},
            {"fps", "60"},
            {"drawCalls", "245"},
            {"triangles", "125000"},
            {"memoryUsage", "128MB"}
        });
    
    // Performance logging with correlation
    logger.SetFrameNumber(1000);
    EDITOR_LOG_INFO(LogCategory::RENDERING, "Rendered frame 1000");
    
    // Crash simulation (commented out for safety)
    // logger.LogCrash("Simulated crash for testing", "Stack trace would go here");
    
    // Export logs
    auto* memoryBuffer = logger.GetMemoryBuffer();
    if (memoryBuffer) {
        auto entries = memoryBuffer->GetEntries(LogLevel::INFO, LogCategory::GENERAL, 100);
        std::cout << "Retrieved " << entries.size() << " log entries for analysis\n";
    }
    
    logger.Flush();
}

/* 
 * Complete Usage Example:
 * 
 * int main() {
 *     // Initialize all systems
 *     RunEnhancedEditorExample();
 *     
 *     // Demonstrate logging
 *     DemonstrateLoggingFeatures();
 *     
 *     return 0;
 * }
 */