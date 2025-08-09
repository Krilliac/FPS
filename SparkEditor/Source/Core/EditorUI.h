/**
 * @file EditorUI.h
 * @brief Core UI management system for the Spark Engine Editor with advanced features
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file defines the main UI controller that manages all editor panels,
 * layout, themes, and user interface interactions with enhanced logging,
 * crash handling, and comprehensive layout management.
 */

#pragma once

// ULTRA AGGRESSIVE BUILD FIX - Include this FIRST
#include "../ultra_build_fix.h"

#include <imgui.h>

#include "EditorLogger.h"  // Include instead of forward declare
#include "EditorLayoutManager.h"
#include "EditorCrashHandler.h"
#include "../Docking/SparkDockingSystem.h"

namespace SparkEditor {

// Forward declarations
class EditorPanel;
class SparkDockingSystem;
class ConsolePanel;

/**
 * @brief Configuration structure for editor settings
 */
struct EditorConfig {
    SparkEditorTypes::String projectPath;
    SparkEditorTypes::String layoutDirectory = "Layouts";
    SparkEditorTypes::String logDirectory = "Logs";
    SparkEditorTypes::String crashDirectory = "Crashes";
    SparkEditorTypes::String themeDirectory = "Themes";
    SparkEditorTypes::String defaultLayout = "Spark Default";
    SparkEditorTypes::String defaultTheme = "Spark Professional";
    bool enableLogging = true;
    bool enableCrashHandler = true;
    bool enableAutoSave = true;
    bool enableExternalConsole = true;
    float autoSaveInterval = 30.0f;
    int maxLogEntries = 10000;
    bool startMaximized = true;
};

/**
 * @brief UI update statistics
 */
struct UIStats {
    float frameTime = 0.0f;
    float averageFrameTime = 0.0f;
    int drawCalls = 0;
    size_t memoryUsage = 0;
    int visiblePanels = 0;
    int totalPanels = 0;
    float layoutSwitchTime = 0.0f;
    SparkEditorTypes::TimePoint lastUpdate;
};

/**
 * @brief Advanced UI management system for the Spark Engine Editor
 * 
 * Coordinates all UI panels, handles layout management, theme application,
 * logging integration, crash handling, and provides a unified interface 
 * for editor UI operations.
 * 
 * Enhanced Features:
 * - Advanced logging system with multiple output targets
 * - Comprehensive layout management with predefined layouts
 * - Crash handling with automatic recovery
 * - Performance profiling and monitoring
 * - Enhanced console with command execution
 * - Real-time engine integration
 * - Asset database integration
 * - Theme management with customization
 */
class EditorUI {
public:
    /**
     * @brief Constructor
     */
    EditorUI();

    /**
     * @brief Destructor
     */
    ~EditorUI();

    /**
     * @brief Initialize the UI system with enhanced features
     * @param config Editor configuration
     * @return true if initialization succeeded, false otherwise
     */
    bool Initialize(const EditorConfig& config = EditorConfig{});

    /**
     * @brief Update UI for the current frame
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void Update(float deltaTime);

    /**
     * @brief Render all UI elements
     */
    void Render();

    /**
     * @brief Shutdown the UI system
     */
    void Shutdown();

    /**
     * @brief Get a panel by type
     * @tparam T Panel type to retrieve
     * @return Pointer to the panel, or nullptr if not found
     */
    template<typename T>
    T* GetPanel() const;

    /**
     * @brief Check if a panel is visible
     * @param panelName Name of the panel to check
     * @return true if the panel is visible, false otherwise
     */
    bool IsPanelVisible(const SparkEditorTypes::String& panelName) const;

    /**
     * @brief Set panel visibility
     * @param panelName Name of the panel
     * @param visible Whether the panel should be visible
     */
    void SetPanelVisible(const SparkEditorTypes::String& panelName, bool visible);

    /**
     * @brief Get layout manager
     * @return Pointer to layout manager
     */
    EditorLayoutManager* GetLayoutManager() const { return m_layoutManager.get(); }

    /**
     * @brief Get logger
     * @return Pointer to logger
     */
    EditorLogger* GetLogger() const { return m_logger.get(); }

    /**
     * @brief Get crash handler
     * @return Pointer to crash handler
     */
    EditorCrashHandler* GetCrashHandler() const { return m_crashHandler; }

    /**
     * @brief Get console panel
     * @return Pointer to console panel
     */
    ConsolePanel* GetConsolePanel() const;

    /**
     * @brief Save current layout to file
     * @param layoutName Name of the layout to save
     * @param description Optional description
     * @return true if layout was saved successfully, false otherwise
     */
    bool SaveLayout(const SparkEditorTypes::String& layoutName, const SparkEditorTypes::String& description = "");

    /**
     * @brief Load layout from file
     * @param layoutName Name of the layout to load
     * @return true if layout was loaded successfully, false otherwise
     */
    bool LoadLayout(const SparkEditorTypes::String& layoutName);

    /**
     * @brief Reset to default layout
     */
    void ResetToDefaultLayout();

    /**
     * @brief Apply theme to the UI
     * @param themeName Name of the theme to apply
     */
    void ApplyTheme(const SparkEditorTypes::String& themeName);

    /**
     * @brief Get current theme name
     * @return Name of the currently applied theme
     */
    const SparkEditorTypes::String& GetCurrentTheme() const { return m_currentTheme; }

    /**
     * @brief Show context menu at current mouse position
     * @param menuItems Map of menu item names to callback functions
     */
    void ShowContextMenu(const SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, SparkEditorTypes::Function<void()>>& menuItems);

    /**
     * @brief Show modal dialog
     * @param title Dialog title
     * @param content Dialog content function
     * @param buttons Map of button names to callback functions
     */
    void ShowModalDialog(const SparkEditorTypes::String& title, 
                        SparkEditorTypes::Function<void()> content,
                        const SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, SparkEditorTypes::Function<void()>>& buttons);

    /**
     * @brief Show notification
     * @param message Notification message
     * @param type Notification type ("info", "warning", "error")
     * @param duration Duration in seconds (0 = persistent)
     */
    void ShowNotification(const SparkEditorTypes::String& message, const SparkEditorTypes::String& type = "info", float duration = 3.0f);

    /**
     * @brief Execute editor command
     * @param command Command to execute
     * @return Command result
     */
    SparkEditorTypes::String ExecuteCommand(const SparkEditorTypes::String& command);

    /**
     * @brief Register custom command
     * @param name Command name
     * @param handler Command handler function
     * @param description Command description
     */
    void RegisterCommand(const SparkEditorTypes::String& name, 
                        SparkEditorTypes::Function<SparkEditorTypes::String(const SparkEditorTypes::Vector<SparkEditorTypes::String>&)> handler,
                        const SparkEditorTypes::String& description = "");

    /**
     * @brief Set frame number for correlation with engine
     * @param frameNumber Current frame number
     */
    void SetFrameNumber(uint64_t frameNumber);

    /**
     * @brief Get UI statistics
     * @return UI performance and usage statistics
     */
    UIStats GetStats() const;

    /**
     * @brief Set engine connection status
     * @param connected Whether engine is connected
     */
    void SetEngineConnected(bool connected);

    /**
     * @brief Check if engine is connected
     * @return true if engine is connected
     */
    bool IsEngineConnected() const { return m_engineConnected; }

    /**
     * @brief Update asset database information
     * @param assetCount Number of assets
     * @param memoryUsage Memory usage in bytes
     */
    void UpdateAssetDatabaseInfo(int assetCount, size_t memoryUsage);

    /**
     * @brief Update scene information
     * @param objectCount Number of scene objects
     * @param selectedCount Number of selected objects
     */
    void UpdateSceneInfo(int objectCount, int selectedCount);

    /**
     * @brief Check if recovery data is available
     * @return true if recovery data exists
     */
    bool HasRecoveryData();

    /**
     * @brief Show recovery dialog
     * @return true if user chose to recover
     */
    bool ShowRecoveryDialog();

    /**
     * @brief Import layout from file
     * @param filePath Path to layout file
     * @return true if import succeeded
     */
    bool ImportLayout(const SparkEditorTypes::String& filePath);

    /**
     * @brief Export current layout to file
     * @param filePath Path to save layout
     * @return true if export succeeded
     */
    bool ExportLayout(const SparkEditorTypes::String& filePath);

    /**
     * @brief Parse command line arguments
     */
    SparkEditorTypes::Vector<SparkEditorTypes::String> ParseCommandLine(const SparkEditorTypes::String& commandLine);

    /**
     * @brief Update performance statistics
     */
    void UpdatePerformanceStats();

    /**
     * @brief Render main menu bar
     */
    void RenderMainMenuBar();

    /**
     * @brief Render status bar
     */
    void RenderStatusBar();

    /**
     * @brief Render notifications
     */
    void RenderNotifications();

    /**
     * @brief Render all panels
     */
    void RenderPanels();

    /**
     * @brief Handle UI events and shortcuts
     */
    void HandleUIEvents();

    /**
     * @brief Update panels
     */
    void UpdatePanels(float deltaTime);

    /**
     * @brief Update statistics
     */
    void UpdateStats(float deltaTime);

    /**
     * @brief Setup crash handler callbacks
     */
    void SetupCrashHandlerCallbacks();

    /**
     * @brief Setup logger callbacks
     */
    void SetupLoggerCallbacks();

    /**
     * @brief Handle layout change event
     */
    void OnLayoutChanged(const SparkEditorTypes::String& layoutName);

    /**
     * @brief Show file dialog
     * @param title Dialog title
     * @param filters File filters
     * @param defaultPath Default path
     * @param save Whether this is a save dialog
     * @return Selected file path
     */
    SparkEditorTypes::String ShowFileDialog(const SparkEditorTypes::String& title, 
                              const char* filters, 
                              const SparkEditorTypes::String& defaultPath, 
                              bool save);

    /**
     * @brief Register built-in commands
     */
    void RegisterBuiltInCommands();

    /**
     * @brief Get available layouts
     * @return Vector of layout names
     */
    SparkEditorTypes::Vector<SparkEditorTypes::String> GetAvailableLayouts() const;

    /**
     * @brief Create and initialize panels
     */
    void CreatePanels();

    /**
     * @brief Register panels with layout manager
     */
    void RegisterPanelsWithLayoutManager();

    /**
     * @brief Render modal dialogs
     */
    void RenderModalDialogs();

private:
    // Core systems
    SparkEditorTypes::UniquePtr<EditorLogger> m_logger;
    SparkEditorTypes::UniquePtr<EditorLayoutManager> m_layoutManager;
    SparkEditorTypes::UniquePtr<SparkDockingSystem> m_dockingSystem;
    EditorCrashHandler* m_crashHandler = nullptr; // Singleton

    // Panel management
    SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, SparkEditorTypes::SharedPtr<EditorPanel>> m_panels;

    // Configuration
    EditorConfig m_config;
    
    // UI state
    bool m_isInitialized = false;
    SparkEditorTypes::String m_currentTheme = "Spark Professional";
    bool m_showDemoWindow = false;
    bool m_firstFrame = true;
    uint64_t m_frameNumber = 0;
    
    // Statistics tracking
    mutable UIStats m_stats;
    SE_CHRONO::steady_clock::time_point m_lastStatsUpdate;
    
    // Status tracking
    bool m_engineConnected = false;
    int m_sceneObjectCount = 0;
    int m_assetDatabaseSize = 0;
    size_t m_assetMemoryUsage = 0;
    
    // Notifications
    struct Notification {
        SparkEditorTypes::String message;
        SparkEditorTypes::String type;
        float duration;
        float timeLeft;
        SparkEditorTypes::TimePoint timestamp;
    };
    SparkEditorTypes::Vector<Notification> m_notifications;
    
    // Dialog state
    struct ModalDialog {
        SparkEditorTypes::String title;
        SparkEditorTypes::Function<void()> content;
        SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, SparkEditorTypes::Function<void()>> buttons;
        bool isOpen = false;
    };
    ModalDialog m_currentDialog;
    
    // Commands
    SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, SparkEditorTypes::Function<SparkEditorTypes::String(const SparkEditorTypes::Vector<SparkEditorTypes::String>&)>> m_commands;
    
    // Performance metrics
    SparkEditorTypes::Vector<float> m_frameTimeHistory;
    static constexpr size_t MAX_FRAME_HISTORY = 60;
    
    // Recovery
    bool m_recoveryDataAvailable = false;
    
    // Additional member variable for selected objects count
    int m_selectedObjectCount = 0;
};

// Template implementation
template<typename T>
T* EditorUI::GetPanel() const {
    for (const auto& panelPair : m_panels) {
        if (auto* typedPanel = dynamic_cast<T*>(panelPair.second.get())) {
            return typedPanel;
        }
    }
    return nullptr;
}

} // namespace SparkEditor