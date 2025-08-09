/**
 * @file EditorLayoutManager.h
 * @brief Advanced layout management system for Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <functional>
#include <imgui.h>
#include "DockPosition.h"  // For DockPosition enum

namespace SparkEditor {

/**
 * @brief Panel configuration
 */
struct PanelConfig {
    std::string name;
    std::string displayName;
    DockPosition dockPosition = DockPosition::Center;
    ImVec2 size = {300, 200};
    ImVec2 position = {0, 0};
    bool isVisible = true;
    bool isFloating = false;
    bool canClose = true;
    bool canDock = true;
    float dockRatio = 0.25f; // Ratio of parent space to occupy
    int tabOrder = 0;
    std::string parentDock; // For nested docking
};

/**
 * @brief Layout configuration
 */
struct LayoutConfig {
    std::string name;
    std::string displayName;
    std::string description;
    std::unordered_map<std::string, PanelConfig> panels;
    ImVec2 mainWindowSize = {1920, 1080};
    bool isBuiltIn = false;
    std::string filePath;
};

/**
 * @brief Layout change event
 */
struct LayoutChangeEvent {
    enum Type {
        PanelMoved,
        PanelResized,
        PanelClosed,
        PanelOpened,
        LayoutLoaded,
        LayoutSaved
    };
    
    Type type;
    std::string panelName;
    std::string layoutName;
};

/**
 * @brief Advanced layout management system
 * 
 * Provides comprehensive layout management with predefined layouts,
 * custom user layouts, persistence, and real-time manipulation.
 */
class EditorLayoutManager {
public:
    /**
     * @brief Constructor
     */
    EditorLayoutManager();

    /**
     * @brief Destructor
     */
    ~EditorLayoutManager();

    /**
     * @brief Initialize the layout manager
     * @param layoutDirectory Directory for storing custom layouts
     * @return true if initialization succeeded
     */
    bool Initialize(const std::string& layoutDirectory = "Layouts");

    /**
     * @brief Shutdown the layout manager
     */
    void Shutdown();

    /**
     * @brief Update layout manager (call once per frame)
     * @param deltaTime Time since last update
     */
    void Update(float deltaTime);

    /**
     * @brief Begin layout frame setup
     */
    void BeginFrame();

    /**
     * @brief End layout frame setup
     */
    void EndFrame();

    /**
     * @brief Apply predefined layout
     * @param layoutName Name of the layout to apply
     * @return true if layout was applied successfully
     */
    bool ApplyLayout(const std::string& layoutName);

    /**
     * @brief Save current layout
     * @param layoutName Name to save the layout as
     * @param description Optional description
     * @return true if layout was saved successfully
     */
    bool SaveCurrentLayout(const std::string& layoutName, const std::string& description = "");

    /**
     * @brief Load custom layout
     * @param layoutName Name of the layout to load
     * @return true if layout was loaded successfully
     */
    bool LoadLayout(const std::string& layoutName);

    /**
     * @brief Delete custom layout
     * @param layoutName Name of the layout to delete
     * @return true if layout was deleted successfully
     */
    bool DeleteLayout(const std::string& layoutName);

    /**
     * @brief Get available layouts
     * @return Vector of layout configurations
     */
    std::vector<LayoutConfig> GetAvailableLayouts() const;

    /**
     * @brief Get current layout name
     * @return Name of currently active layout
     */
    const std::string& GetCurrentLayoutName() const { return m_currentLayoutName; }

    /**
     * @brief Register a panel
     * @param config Panel configuration
     */
    void RegisterPanel(const PanelConfig& config);

    /**
     * @brief Unregister a panel
     * @param panelName Name of the panel to unregister
     */
    void UnregisterPanel(const std::string& panelName);

    /**
     * @brief Get panel configuration
     * @param panelName Name of the panel
     * @return Pointer to panel config, or nullptr if not found
     */
    const PanelConfig* GetPanelConfig(const std::string& panelName) const;

    /**
     * @brief Update panel configuration
     * @param panelName Name of the panel
     * @param config New configuration
     */
    void UpdatePanelConfig(const std::string& panelName, const PanelConfig& config);

    /**
     * @brief Set panel visibility
     * @param panelName Name of the panel
     * @param visible Visibility state
     */
    void SetPanelVisible(const std::string& panelName, bool visible);

    /**
     * @brief Check if panel is visible
     * @param panelName Name of the panel
     * @return true if panel is visible
     */
    bool IsPanelVisible(const std::string& panelName) const;

    /**
     * @brief Set panel floating state
     * @param panelName Name of the panel
     * @param floating Floating state
     */
    void SetPanelFloating(const std::string& panelName, bool floating);

    /**
     * @brief Begin panel rendering
     * @param panelName Name of the panel
     * @return true if panel should be rendered
     */
    bool BeginPanel(const std::string& panelName);

    /**
     * @brief End panel rendering
     */
    void EndPanel();

    /**
     * @brief Reset to default layout
     */
    void ResetToDefault();

    /**
     * @brief Set layout change callback
     * @param callback Callback function for layout changes
     */
    void SetLayoutChangeCallback(std::function<void(const LayoutChangeEvent&)> callback);

    /**
     * @brief Enable/disable automatic layout saving
     * @param enabled Auto-save enabled
     * @param interval Save interval in seconds
     */
    void SetAutoSave(bool enabled, float interval = 30.0f);

    /**
     * @brief Import layout from file
     * @param filePath Path to layout file
     * @return true if import succeeded
     */
    bool ImportLayout(const std::string& filePath);

    /**
     * @brief Export layout to file
     * @param layoutName Name of layout to export
     * @param filePath Path to save layout file
     * @return true if export succeeded
     */
    bool ExportLayout(const std::string& layoutName, const std::string& filePath);

    /**
     * @brief Get layout statistics
     */
    struct LayoutStats {
        int totalPanels = 0;
        int visiblePanels = 0;
        int floatingPanels = 0;
        int dockedPanels = 0;
        std::string mostUsedLayout;
        float averageLayoutSwitchTime = 0.0f;
    };
    LayoutStats GetLayoutStats() const;

private:
    /**
     * @brief Create predefined layouts
     */
    void CreatePredefinedLayouts();

    /**
     * @brief Create Spark Default layout
     */
    LayoutConfig CreateSparkDefaultLayout();

    /**
     * @brief Create Engine Debug layout
     */
    LayoutConfig CreateEngineDebugLayout();

    /**
     * @brief Create Performance layout
     */
    LayoutConfig CreatePerformanceLayout();

    /**
     * @brief Create Asset Creation layout
     */
    LayoutConfig CreateAssetCreationLayout();

    /**
     * @brief Create Lighting Design layout
     */
    LayoutConfig CreateLightingDesignLayout();

    /**
     * @brief Create Animation layout
     */
    LayoutConfig CreateAnimationLayout();

    /**
     * @brief Setup docking for layout
     * @param layout Layout configuration
     */
    void SetupDocking(const LayoutConfig& layout);

    /**
     * @brief Create dock node
     * @param position Dock position
     * @param size Dock size
     * @param parentId Parent dock ID
     * @return Dock node ID
     */
    ImGuiID CreateDockNode(DockPosition position, const ImVec2& size, ImGuiID parentId = 0);

    /**
     * @brief Dock panel to node
     * @param panelName Panel name
     * @param nodeId Dock node ID
     */
    void DockPanelToNode(const std::string& panelName, ImGuiID nodeId);

    /**
     * @brief Save layout to file
     * @param layout Layout configuration
     * @param filePath File path to save to
     * @return true if save succeeded
     */
    bool SaveLayoutToFile(const LayoutConfig& layout, const std::string& filePath);

    /**
     * @brief Load layout from file
     * @param filePath File path to load from
     * @return Loaded layout configuration
     */
    LayoutConfig LoadLayoutFromFile(const std::string& filePath);

    /**
     * @brief Capture current panel states
     * @return Current layout configuration
     */
    LayoutConfig CaptureCurrentLayout();

    /**
     * @brief Update layout statistics
     */
    void UpdateLayoutStats();

    /**
     * @brief Handle layout change event
     * @param event Layout change event
     */
    void OnLayoutChanged(const LayoutChangeEvent& event);

private:
    // Layout storage
    std::unordered_map<std::string, LayoutConfig> m_layouts;
    std::unordered_map<std::string, PanelConfig> m_panels;
    std::string m_currentLayoutName = "Spark Default";
    std::string m_layoutDirectory;

    // State
    bool m_isInitialized = false;
    bool m_layoutChanged = false;
    bool m_firstFrame = true;
    ImGuiID m_mainDockId = 0;

    // Auto-save
    bool m_autoSaveEnabled = true;
    float m_autoSaveInterval = 30.0f;
    float m_autoSaveTimer = 0.0f;

    // Callbacks
    std::function<void(const LayoutChangeEvent&)> m_layoutChangeCallback;

    // Statistics
    mutable LayoutStats m_stats;
    std::unordered_map<std::string, int> m_layoutUsageCount;
    std::chrono::steady_clock::time_point m_lastLayoutSwitchTime;

    // Dock node IDs for predefined positions
    ImGuiID m_leftDockId = 0;
    ImGuiID m_rightDockId = 0;
    ImGuiID m_topDockId = 0;
    ImGuiID m_bottomDockId = 0;
    ImGuiID m_centerDockId = 0;
};

} // namespace SparkEditor