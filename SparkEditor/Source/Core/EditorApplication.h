/**
 * @file EditorApplication.h
 * @brief Core editor application class and configuration
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file defines the main editor application class that manages the entire
 * editor lifecycle, including UI, engine communication, project management,
 * and all editor subsystems.
 */

#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <chrono>
#include <memory>
#include <string>

// Forward declarations
namespace SparkEditor {
    class EditorUI;
    class SceneManager;
    class AssetDatabase;
    class EngineInterface;
    class ProjectManager;
}

namespace SparkEditor {

/**
 * @brief Configuration structure for editor initialization
 * 
 * Contains all necessary configuration parameters for starting the editor,
 * including window settings, project paths, and runtime options.
 */
struct EditorConfig {
    HINSTANCE hInstance = nullptr;          ///< Windows application instance handle
    int nCmdShow = SW_SHOWDEFAULT;         ///< Window display mode
    std::string projectPath;               ///< Path to project to open on startup
    std::string scenePath;                 ///< Path to scene to open on startup
    bool enableProfiling = false;          ///< Enable performance profiling
    bool verboseLogging = false;           ///< Enable verbose debug logging
    bool enableEngineConnection = true;    ///< Connect to engine for live editing
    int windowWidth = 1920;               ///< Initial window width
    int windowHeight = 1080;              ///< Initial window height
    bool maximizeOnStart = true;          ///< Maximize window on startup
    std::string themeName = "Dark";       ///< UI theme name
    std::string layoutName = "Default";   ///< UI layout name
};

/**
 * @brief Main editor application class
 * 
 * The central controller for the entire Spark Engine Editor application.
 * Manages initialization, shutdown, main loop, and coordination between
 * all editor subsystems.
 * 
 * Features:
 * - Window management and DirectX rendering setup
 * - UI system coordination (Dear ImGui with docking)
 * - Engine communication and live editing
 * - Project and scene management
 * - Asset pipeline integration
 * - Performance monitoring and profiling
 * - Event handling and input processing
 */
class EditorApplication {
public:
    /**
     * @brief Constructor
     * 
     * Initializes the editor application in a default state.
     * Call Initialize() to fully set up the application.
     */
    EditorApplication();

    /**
     * @brief Destructor
     * 
     * Ensures proper cleanup of all editor resources and subsystems.
     * Automatically calls Shutdown() if not already called.
     */
    ~EditorApplication();

    /**
     * @brief Initialize the editor application
     * 
     * Sets up all editor subsystems, creates the main window, initializes
     * DirectX rendering, and prepares the UI framework.
     * 
     * @param config Configuration parameters for initialization
     * @return true if initialization succeeded, false otherwise
     */
    bool Initialize(const EditorConfig& config);

    /**
     * @brief Run the main application loop
     * 
     * Enters the main message loop and runs until the application is closed.
     * Handles Windows messages, updates all subsystems, and renders the UI.
     * 
     * @return Application exit code (0 for normal exit)
     */
    int Run();

    /**
     * @brief Shutdown the editor application
     * 
     * Cleanly shuts down all subsystems, saves any pending changes,
     * and releases all resources.
     */
    void Shutdown();

    /**
     * @brief Check if the application is running
     * @return true if the application is in the main loop, false otherwise
     */
    bool IsRunning() const { return m_isRunning; }

    /**
     * @brief Request application exit
     * 
     * Initiates a graceful shutdown sequence. The application will exit
     * after completing the current frame and saving any pending changes.
     */
    void RequestExit();

    /**
     * @brief Get the current project path
     * @return Path to the currently opened project, or empty string if none
     */
    const std::string& GetCurrentProjectPath() const { return m_currentProjectPath; }

    /**
     * @brief Get the main window handle
     * @return Windows handle to the main editor window
     */
    HWND GetMainWindow() const { return m_hwnd; }

    /**
     * @brief Get application performance metrics
     * @return Structure containing FPS, frame time, and memory usage
     */
    struct PerformanceMetrics {
        float fps = 0.0f;
        float frameTime = 0.0f;
        float uiTime = 0.0f;
        float renderTime = 0.0f;
        size_t memoryUsage = 0;
        size_t gpuMemoryUsage = 0;
    };
    PerformanceMetrics GetPerformanceMetrics() const;

private:
    /**
     * @brief Create and setup the main application window
     * @param config Configuration containing window parameters
     * @return true if window creation succeeded, false otherwise
     */
    bool CreateMainWindow(const EditorConfig& config);

    /**
     * @brief Initialize DirectX 11 rendering system
     * @return true if DirectX initialization succeeded, false otherwise
     */
    bool InitializeDirectX();

    /**
     * @brief Initialize Dear ImGui UI framework
     * @return true if ImGui initialization succeeded, false otherwise
     */
    bool InitializeImGui();

    /**
     * @brief Initialize all editor subsystems
     * @param config Configuration parameters
     * @return true if all subsystems initialized successfully, false otherwise
     */
    bool InitializeSubsystems(const EditorConfig& config);

    /**
     * @brief Process Windows messages
     * @return true to continue running, false to exit
     */
    bool ProcessMessages();

    /**
     * @brief Update all editor subsystems for the current frame
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void Update(float deltaTime);

    /**
     * @brief Render the editor UI and viewport
     */
    void Render();

    /**
     * @brief Update performance metrics
     */
    void UpdatePerformanceMetrics();

    /**
     * @brief Windows message procedure
     * @param hwnd Window handle
     * @param msg Message identifier
     * @param wParam Message parameter
     * @param lParam Message parameter
     * @return Message processing result
     */
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Handle window resize events
     * @param width New window width
     * @param height New window height
     */
    void OnWindowResize(int width, int height);

    /**
     * @brief Handle application shutdown request
     * @return true to allow shutdown, false to cancel
     */
    bool OnShutdownRequested();

private:
    // Window and DirectX resources
    HWND m_hwnd = nullptr;                                    ///< Main window handle
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;          ///< DirectX 11 device
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;  ///< DirectX 11 context
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;     ///< DXGI swap chain
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;   ///< Render target view
    
    // Editor subsystems
    std::unique_ptr<EditorUI> m_ui;                          ///< UI management system
    std::unique_ptr<SceneManager> m_sceneManager;            ///< Scene file management
    std::unique_ptr<AssetDatabase> m_assetDatabase;          ///< Asset management
    std::unique_ptr<EngineInterface> m_engineInterface;      ///< Engine communication
    std::unique_ptr<ProjectManager> m_projectManager;        ///< Project management
    
    // Application state
    bool m_isRunning = false;                                ///< Main loop running flag
    bool m_isInitialized = false;                           ///< Initialization complete flag
    std::string m_currentProjectPath;                       ///< Currently opened project
    EditorConfig m_config;                                   ///< Current configuration
    
    // Performance tracking
    mutable PerformanceMetrics m_performanceMetrics;        ///< Current performance data
    std::chrono::high_resolution_clock::time_point m_lastFrameTime; ///< Last frame timestamp
    std::chrono::high_resolution_clock::time_point m_startTime;     ///< Application start time
    
    // Window dimensions
    int m_windowWidth = 1920;                               ///< Current window width
    int m_windowHeight = 1080;                              ///< Current window height
};

} // namespace SparkEditor