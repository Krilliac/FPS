/**
 * @file EditorUI.h
 * @brief Core UI management system for the Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file defines the main UI controller that manages all editor panels,
 * layout, themes, and user interface interactions.
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

namespace SparkEditor {

/**
 * @brief Main UI management system for the editor
 * 
 * Coordinates all UI panels, handles layout management, theme application,
 * and provides a unified interface for editor UI operations.
 * 
 * Features:
 * - Panel management with docking support
 * - Layout saving and restoration
 * - Theme management
 * - Menu bar and toolbar coordination
 * - Event routing between panels
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
     * @brief Initialize the UI system
     * @return true if initialization succeeded, false otherwise
     */
    bool Initialize();

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
     * @brief Check if a panel is visible
     * @param panelName Name of the panel to check
     * @return true if the panel is visible, false otherwise
     */
    bool IsPanelVisible(const std::string& panelName) const;

    /**
     * @brief Set panel visibility
     * @param panelName Name of the panel
     * @param visible Whether the panel should be visible
     */
    void SetPanelVisible(const std::string& panelName, bool visible);

    /**
     * @brief Save current layout to file
     * @param layoutName Name of the layout to save
     * @return true if layout was saved successfully, false otherwise
     */
    bool SaveLayout(const std::string& layoutName);

    /**
     * @brief Load layout from file
     * @param layoutName Name of the layout to load
     * @return true if layout was loaded successfully, false otherwise
     */
    bool LoadLayout(const std::string& layoutName);

    /**
     * @brief Reset to default layout
     */
    void ResetToDefaultLayout();

    /**
     * @brief Get list of available layouts
     * @return Vector of layout names
     */
    std::vector<std::string> GetAvailableLayouts() const;

    /**
     * @brief Apply theme to the UI
     * @param themeName Name of the theme to apply
     */
    void ApplyTheme(const std::string& themeName);

    /**
     * @brief Get current theme name
     * @return Name of the currently applied theme
     */
    const std::string& GetCurrentTheme() const { return m_currentTheme; }

    /**
     * @brief Show context menu at current mouse position
     * @param menuItems Map of menu item names to callback functions
     */
    void ShowContextMenu(const std::unordered_map<std::string, std::function<void()>>& menuItems);

    /**
     * @brief Show modal dialog
     * @param title Dialog title
     * @param content Dialog content function
     * @param buttons Map of button names to callback functions
     */
    void ShowModalDialog(const std::string& title, 
                        std::function<void()> content,
                        const std::unordered_map<std::string, std::function<void()>>& buttons);

private:
    /**
     * @brief Create and initialize all editor panels
     */
    void CreatePanels();

    /**
     * @brief Setup subsystem integration
     */
    void SetupSubsystemIntegration();

    /**
     * @brief Update subsystem integration
     * @param deltaTime Time since last update
     */
    void UpdateSubsystemIntegration(float deltaTime);

    /**
     * @brief Render Spark Engine menu bar
     */
    void RenderSparkMenuBar();

    /**
     * @brief Render Spark Engine toolbar
     */
    void RenderSparkToolbar();

    /**
     * @brief Render Spark Engine status bar
     */
    void RenderSparkStatusBar();

    /**
     * @brief Render Spark Engine dialog
     */
    void RenderSparkDialog();

    /**
     * @brief Render all panels with Spark styling
     */
    void RenderSparkPanels();

    /**
     * @brief Setup default docking layout
     */
    void SetupDefaultDocking();

    /**
     * @brief Handle UI events and shortcuts
     */
    void HandleUIEvents();

    /**
     * @brief Update panel states
     */
    void UpdatePanels(float deltaTime);

    /**
     * @brief Show file dialog
     * @param title Dialog title
     * @param filters File filters (e.g., "Scene Files (*.scene)\0*.scene\0")
     * @param defaultPath Default path to open
     * @param save Whether this is a save dialog (true) or open dialog (false)
     * @return Selected file path, or empty string if cancelled
     */
    std::string ShowFileDialog(const std::string& title, 
                              const char* filters, 
                              const std::string& defaultPath, 
                              bool save);

private:
    // Panel visibility states
    std::unordered_map<std::string, bool> m_panelVisibility; ///< Panel visibility map
    
    // UI state
    bool m_isInitialized = false;                           ///< Initialization complete flag
    std::string m_currentTheme = "Unity Pro";               ///< Current theme name
    bool m_showDemoWindow = false;                          ///< Show ImGui demo window
    bool m_firstFrame = true;                               ///< First frame flag for setup
    
    // Engine integration state
    bool m_engineConnected = false;                         ///< Engine connection status
    int m_assetDatabaseSize = 0;                            ///< Number of assets in database
    int m_sceneObjectCount = 0;                             ///< Number of objects in scene
    
    // Dialog state
    struct ModalDialog {
        std::string title;
        std::function<void()> content;
        std::unordered_map<std::string, std::function<void()>> buttons;
        bool isOpen = false;
    };
    ModalDialog m_currentDialog;                            ///< Current modal dialog
    
    // Performance metrics for UI
    struct UIMetrics {
        float lastFrameTime = 0.0f;
        int drawCalls = 0;
        size_t memoryUsage = 0;
    };
    mutable UIMetrics m_uiMetrics;                          ///< UI performance metrics
};

} // namespace SparkEditor