/**
 * @file SparkDockingSystem.h
 * @brief Professional-grade custom docking system for Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file implements a complete custom docking system designed specifically 
 * for the Spark Engine Editor, providing professional window management without 
 * external dependencies.
 */

#pragma once

// ULTRA AGGRESSIVE BUILD FIX - Include this FIRST
#include "../ultra_build_fix.h"

#include <imgui.h>
#include "DockPosition.h"

namespace SparkEditor {

/**
 * @brief Represents a dock operation during drag and drop
 */
struct DockOperation {
    SparkEditorTypes::String panelId;
    DockPosition targetPosition;
    SparkEditorTypes::String targetZoneId;
    ImVec2 dropPosition;
    bool isValid = false;
    
    DockOperation() = default;
    DockOperation(const SparkEditorTypes::String& panel, DockPosition pos, const SparkEditorTypes::String& zone, ImVec2 drop)
        : panelId(panel), targetPosition(pos), targetZoneId(zone), dropPosition(drop), isValid(true) {}
};

/**
 * @brief Represents a dockable zone in the editor
 */
struct DockZone {
    SparkEditorTypes::String id;
    DockPosition position;
    float x, y, width, height;
    float splitRatio = 0.5f;
    bool allowTabbing = true;
    bool allowSplitting = true;
    SparkEditorTypes::Vector<SparkEditorTypes::String> panelIds;
    int activeTabIndex = 0;
    bool isVisible = true;
    
    DockZone() = default;
    DockZone(const SparkEditorTypes::String& zoneId, DockPosition pos, float x_, float y_, float w_, float h_)
        : id(zoneId), position(pos), x(x_), y(y_), width(w_), height(h_) {}
        
    bool ContainsPoint(ImVec2 point) const {
        return point.x >= x && point.x <= (x + width) && 
               point.y >= y && point.y <= (y + height);
    }
};

/**
 * @brief Layout snapshot for undo/redo functionality
 */
struct LayoutSnapshot {
    SparkEditorTypes::Vector<DockZone> zones;
    SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, bool> panelVisibility;
    SparkEditorTypes::TimePoint timestamp;
    SparkEditorTypes::String description;
};

/**
 * @brief Professional custom docking system for Spark Engine Editor
 * 
 * Provides a complete docking solution with the following features:
 * - Drag and drop panel docking with visual feedback
 * - Multiple dock zones (Left, Right, Top, Bottom, Center)
 * - Tabbed panel groups with drag-and-drop reordering
 * - Split layouts with adjustable splitters
 * - Floating windows with docking capabilities
 * - Visual docking guides and drop zone indicators
 * - Layout serialization and restoration
 * - Undo/redo support for layout changes
 * - Grid snapping for precise positioning
 * - Professional visual styling
 */
class SparkDockingSystem {
public:
    /**
     * @brief Constructor
     */
    SparkDockingSystem();

    /**
     * @brief Destructor
     */
    ~SparkDockingSystem();

    /**
     * @brief Initialize the docking system
     * @param viewportSize Initial viewport size
     * @return true if initialization succeeded
     */
    bool Initialize(ImVec2 viewportSize);

    /**
     * @brief Update docking system for current frame
     * @param deltaTime Time elapsed since last frame
     */
    void Update(float deltaTime);

    /**
     * @brief Begin docking frame
     * @param viewportSize Current viewport size
     */
    void BeginFrame(ImVec2 viewportSize);

    /**
     * @brief End docking frame and render guides
     */
    void EndFrame();

    /**
     * @brief Render a docked panel
     * @param panelId Unique identifier for the panel
     * @param title Display title of the panel
     * @param isOpen Pointer to boolean controlling panel visibility
     * @param renderContent Function to render panel content
     * @param flags Additional ImGui window flags
     * @return true if panel was rendered and is visible
     */
    bool RenderDockedPanel(const SparkEditorTypes::String& panelId, 
                          const SparkEditorTypes::String& title,
                          bool* isOpen,
                          SparkEditorTypes::Function<void()> renderContent,
                          ImGuiWindowFlags flags = 0);

    /**
     * @brief Dock a panel to a specific zone
     * @param panelId Panel to dock
     * @param position Target dock position
     * @param zoneId Target zone ID (optional)
     * @return true if docking succeeded
     */
    bool DockPanel(const SparkEditorTypes::String& panelId, DockPosition position, const SparkEditorTypes::String& zoneId = "");

    /**
     * @brief Undock a panel (make it floating)
     * @param panelId Panel to undock
     * @return true if undocking succeeded
     */
    bool UndockPanel(const SparkEditorTypes::String& panelId);

    /**
     * @brief Create a new dock zone
     * @param zoneId Unique identifier for the zone
     * @param position Zone position
     * @param bounds Zone bounds
     * @return true if zone was created successfully
     */
    bool CreateZone(const SparkEditorTypes::String& zoneId, DockPosition position, ImVec4 bounds);

    /**
     * @brief Remove a dock zone
     * @param zoneId Zone to remove
     * @return true if zone was removed
     */
    bool RemoveZone(const SparkEditorTypes::String& zoneId);

    /**
     * @brief Get zone containing a panel
     * @param panelId Panel to find
     * @return Pointer to zone containing the panel, or nullptr
     */
    DockZone* GetZoneForPanel(const SparkEditorTypes::String& panelId);

    /**
     * @brief Save current layout to string
     * @return Serialized layout data
     */
    SparkEditorTypes::String SaveLayout() const;

    /**
     * @brief Load layout from string
     * @param layoutData Serialized layout data
     * @return true if layout was loaded successfully
     */
    bool LoadLayout(const SparkEditorTypes::String& layoutData);

    /**
     * @brief Reset to default layout
     */
    void ResetLayout();

    /**
     * @brief Create layout snapshot for undo
     * @param description Description of the operation
     */
    void CreateSnapshot(const SparkEditorTypes::String& description = "");

    /**
     * @brief Undo last layout change
     * @return true if undo was performed
     */
    bool UndoLayout();

    /**
     * @brief Redo last undone layout change
     * @return true if redo was performed
     */
    bool RedoLayout();

    /**
     * @brief Enable/disable docking guides
     * @param enabled Whether guides should be shown
     */
    void SetDockingGuidesEnabled(bool enabled) { m_showDockingGuides = enabled; }

    /**
     * @brief Check if docking guides are enabled
     */
    bool AreDockingGuidesEnabled() const { return m_showDockingGuides; }

    /**
     * @brief Enable/disable grid snapping
     * @param enabled Whether grid snapping should be active
     */
    void SetGridSnapEnabled(bool enabled) { m_gridSnapEnabled = enabled; }

    /**
     * @brief Set grid snap size
     * @param size Grid size in pixels
     */
    void SetGridSize(float size) { m_gridSize = size; }

    /**
     * @brief Get all dock zones
     * @return Vector of all dock zones
     */
    const SparkEditorTypes::Vector<DockZone>& GetZones() const { return m_zones; }

    /**
     * @brief Get zone by ID
     * @param zoneId Zone identifier
     * @return Pointer to zone, or nullptr if not found
     */
    DockZone* GetZone(const SparkEditorTypes::String& zoneId);

    /**
     * @brief Check if a panel is docked
     * @param panelId Panel to check
     * @return true if panel is docked
     */
    bool IsPanelDocked(const SparkEditorTypes::String& panelId) const;

    /**
     * @brief Get docking statistics
     */
    struct DockingStats {
        int totalZones = 0;
        int visibleZones = 0;
        int dockedPanels = 0;
        int floatingPanels = 0;
        float layoutSwitchTime = 0.0f;
        int undoStackSize = 0;
        int redoStackSize = 0;
    };
    DockingStats GetStats() const;

private:
    /**
     * @brief Initialize default zones
     */
    void InitializeDefaultZones();

    /**
     * @brief Update zone positions based on viewport
     */
    void UpdateZonePositions();

    /**
     * @brief Handle drag and drop operations
     */
    void HandleDragAndDrop();

    /**
     * @brief Calculate dock operation for current mouse position
     */
    DockOperation CalculateDockOperation() const;

    /**
     * @brief Render docking guides
     */
    void RenderDockingGuides();

    /**
     * @brief Render dock preview
     */
    void RenderDockPreview(const DockOperation& operation);

    /**
     * @brief Render tabbed zone
     */
    void RenderTabbedZone(DockZone& zone);

    /**
     * @brief Render splitters
     */
    void RenderSplitters();

    /**
     * @brief Apply snap to grid
     */
    ImVec2 ApplyGridSnap(ImVec2 position) const;

    /**
     * @brief Find zone at position
     */
    DockZone* FindZoneAtPosition(ImVec2 position);

    /**
     * @brief Serialize zone to string
     */
    SparkEditorTypes::String SerializeZone(const DockZone& zone) const;

    /**
     * @brief Deserialize zone from string
     */
    bool DeserializeZone(const SparkEditorTypes::String& data, DockZone& zone) const;

private:
    // Core state
    bool m_isInitialized = false;
    ImVec2 m_viewportSize;
    SparkEditorTypes::Vector<DockZone> m_zones;
    
    // Drag and drop state
    bool m_isDragging = false;
    SparkEditorTypes::String m_draggedPanelId;
    ImVec2 m_dragStartPos;
    DockOperation m_currentDockOperation;
    
    // Visual settings
    bool m_showDockingGuides = true;
    bool m_gridSnapEnabled = false;
    float m_gridSize = 20.0f;
    
    // Colors and styling
    ImU32 m_dockGuideColor = IM_COL32(100, 150, 255, 200);     // Spark Blue
    ImU32 m_dockPreviewColor = IM_COL32(100, 150, 255, 100);   // Transparent Blue
    ImU32 m_splitterColor = IM_COL32(80, 80, 80, 255);         // Professional Gray
    ImU32 m_splitterHoverColor = IM_COL32(120, 120, 120, 255);
    ImU32 m_splitterActiveColor = IM_COL32(150, 150, 150, 255);
    
    // Layout management
    SparkEditorTypes::Vector<LayoutSnapshot> m_undoStack;
    SparkEditorTypes::Vector<LayoutSnapshot> m_redoStack;
    static constexpr size_t MAX_UNDO_STACK_SIZE = 50;
    
    // Panel tracking
    SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, bool> m_panelVisibility;
    
    // Performance
    float m_lastLayoutSwitchTime = 0.0f;
    SparkEditorTypes::TimePoint m_lastUpdateTime;
};

} // namespace SparkEditor