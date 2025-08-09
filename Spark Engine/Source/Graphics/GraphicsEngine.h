/**
 * @file GraphicsEngine.h
 * @brief DirectX 11 graphics engine with comprehensive console integration
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class manages the entire DirectX 11 rendering pipeline including device creation,
 * swap chain management, render target setup, depth buffer configuration, frame
 * rendering operations, and comprehensive console integration for real-time graphics
 * debugging and parameter adjustment.
 */

#pragma once

#include "Utils/Assert.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <dxgi1_3.h>     // IDXGIFactory2, IDXGISwapChain1
#include <dxgidebug.h>   // DXGIGetDebugInterface1, IDXGIInfoQueue
#include "..\Core\framework.h"  // XMFLOAT3, XMMATRIX, HRESULT
#include <functional>    // std::function
#include <mutex>         // std::mutex
#include <chrono>        // For performance timing

using Microsoft::WRL::ComPtr;

/**
 * @brief Core DirectX 11 graphics engine with console integration
 * 
 * The GraphicsEngine class encapsulates all DirectX 11 functionality required for
 * rendering 3D graphics in the Spark Engine. It manages device initialization,
 * resource creation, provides a clean interface for rendering operations, and
 * includes comprehensive console integration for real-time debugging and tuning.
 * 
 * @note This class follows RAII principles and automatically handles resource cleanup
 * @warning Ensure Initialize() is called before any rendering operations
 */
class GraphicsEngine
{
public:
    /**
     * @brief Default constructor
     * 
     * Initializes member variables to safe default values. The actual DirectX
     * device and resources are created in the Initialize() method.
     */
    GraphicsEngine();

    /**
     * @brief Destructor
     * 
     * Automatically calls Shutdown() to ensure proper resource cleanup.
     */
    ~GraphicsEngine();

    /**
     * @brief Initialize the graphics engine with DirectX 11
     * 
     * Creates the DirectX 11 device, device context, swap chain, render target view,
     * and depth stencil view. Also sets up debug output filters and viewport.
     * 
     * @param hWnd Handle to the window for rendering
     * @return HRESULT indicating success or failure of initialization
     * @note Must be called before any rendering operations
     * @warning Calling this multiple times without Shutdown() will cause resource leaks
     */
    HRESULT Initialize(HWND hWnd);

    /**
     * @brief Clean up all DirectX resources
     * 
     * Releases all COM interfaces and resets internal state. Safe to call
     * multiple times.
     */
    void Shutdown();

    /**
     * @brief Begin a new rendering frame
     * 
     * Clears the render target and depth buffer, preparing for new frame rendering.
     * Should be called at the start of each frame before any draw operations.
     */
    void BeginFrame();

    /**
     * @brief Complete the current frame and present to screen
     * 
     * Presents the completed frame to the display. Should be called after all
     * draw operations for the current frame are complete.
     */
    void EndFrame();

    /**
     * @brief Handle window resize events
     * 
     * Recreates the swap chain buffers and adjusts the viewport to match
     * the new window dimensions.
     * 
     * @param width New window width in pixels
     * @param height New window height in pixels
     */
    void OnResize(UINT width, UINT height);

    /**
     * @brief Get the DirectX 11 device
     * @return Pointer to the ID3D11Device interface
     * @note Returns nullptr if Initialize() hasn't been called successfully
     */
    ID3D11Device* GetDevice()     const { return m_device.Get(); }

    /**
     * @brief Get the DirectX 11 device context
     * @return Pointer to the ID3D11DeviceContext interface  
     * @note Returns nullptr if Initialize() hasn't been called successfully
     */
    ID3D11DeviceContext* GetContext()    const { return m_context.Get(); }

    /**
     * @brief Get the current window width
     * @return Window width in pixels
     */
    UINT                 GetWindowWidth() const { return m_windowWidth; }

    /**
     * @brief Get the current window height
     * @return Window height in pixels
     */
    UINT                 GetWindowHeight() const { return m_windowHeight; }

    /**
     * @brief Get the DXGI swap chain
     * @return Pointer to the IDXGISwapChain interface
     * @note Returns nullptr if Initialize() hasn't been called successfully
     */
    IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }

    // ============================================================================
    // CONSOLE INTEGRATION METHODS - Graphics Engine Control
    // ============================================================================

    /**
     * @brief Graphics performance metrics structure
     */
    struct GraphicsMetrics {
        float frameTime;        ///< Frame time in milliseconds
        float renderTime;       ///< Render time in milliseconds
        float presentTime;      ///< Present time in milliseconds
        int drawCalls;          ///< Number of draw calls per frame
        int triangles;          ///< Number of triangles rendered
        int vertices;           ///< Number of vertices processed
        size_t textureMemory;   ///< Texture memory usage in bytes
        size_t bufferMemory;    ///< Buffer memory usage in bytes
        float gpuUsage;         ///< GPU usage percentage (if available)
        bool vsyncEnabled;      ///< VSync state
        bool wireframeMode;     ///< Wireframe rendering state
        bool debugMode;         ///< Debug rendering state
    };

    /**
     * @brief Graphics settings structure for console control
     */
    struct GraphicsSettings {
        bool vsyncEnabled;      ///< Enable/disable VSync
        bool wireframeMode;     ///< Enable/disable wireframe rendering
        bool debugMode;         ///< Enable/disable debug rendering
        bool showFPS;           ///< Show FPS counter
        float clearColor[4];    ///< Background clear color (RGBA)
        int msaaSamples;        ///< MSAA sample count (1, 2, 4, 8)
        bool enableGPUTiming;   ///< Enable GPU timing queries
        float renderScale;      ///< Render scale factor (0.5-2.0)
    };

    /**
     * @brief Enable/disable VSync via console
     * @param enabled true to enable VSync, false to disable
     */
    void Console_SetVSync(bool enabled);

    /**
     * @brief Enable/disable wireframe rendering via console
     * @param enabled true for wireframe mode, false for solid rendering
     */
    void Console_SetWireframeMode(bool enabled);

    /**
     * @brief Set background clear color via console
     * @param r Red component (0.0-1.0)
     * @param g Green component (0.0-1.0)
     * @param b Blue component (0.0-1.0)
     * @param a Alpha component (0.0-1.0)
     */
    void Console_SetClearColor(float r, float g, float b, float a);

    /**
     * @brief Set render scale factor via console
     * @param scale Scale factor (0.5-2.0, 1.0 = native resolution)
     */
    void Console_SetRenderScale(float scale);

    /**
     * @brief Enable/disable debug rendering mode via console
     * @param enabled true to enable debug mode, false to disable
     */
    void Console_SetDebugMode(bool enabled);

    /**
     * @brief Take a screenshot via console
     * @param filename Output filename (optional, auto-generated if empty)
     * @return true if screenshot was taken successfully
     */
    bool Console_TakeScreenshot(const std::string& filename = "");

    /**
     * @brief Force a full graphics device reset via console
     */
    void Console_ResetDevice();

    /**
     * @brief Get comprehensive graphics metrics (console integration)
     * @return GraphicsMetrics structure with current performance data
     */
    GraphicsMetrics Console_GetMetrics() const;

    /**
     * @brief Get current graphics settings (console integration)
     * @return GraphicsSettings structure with current settings
     */
    GraphicsSettings Console_GetSettings() const;

    /**
     * @brief Apply graphics settings from console
     * @param settings GraphicsSettings structure with new settings
     */
    void Console_ApplySettings(const GraphicsSettings& settings);

    /**
     * @brief Reset graphics settings to defaults via console
     */
    void Console_ResetToDefaults();

    /**
     * @brief Register console state change callback
     * @param callback Function to call when graphics state changes
     */
    void Console_RegisterStateCallback(std::function<void()> callback);

    /**
     * @brief Enable/disable GPU performance queries via console
     * @param enabled true to enable GPU timing, false to disable
     */
    void Console_SetGPUTiming(bool enabled);

    /**
     * @brief Get VRAM usage information via console
     * @return VRAM usage in bytes, or 0 if unavailable
     */
    size_t Console_GetVRAMUsage() const;

    /**
     * @brief Force garbage collection of graphics resources via console
     */
    void Console_ForceGarbageCollection();

private:
    /**
     * @brief Create DirectX device and swap chain
     * @param hWnd Window handle for the swap chain
     * @return HRESULT indicating success or failure
     */
    HRESULT CreateDeviceAndSwapChain(HWND hWnd);

    /**
     * @brief Create the render target view from swap chain back buffer
     * @return HRESULT indicating success or failure
     */
    HRESULT CreateRenderTargetView();

    /**
     * @brief Create depth stencil buffer and view
     * @return HRESULT indicating success or failure
     */
    HRESULT CreateDepthStencilView();

    /**
     * @brief Configure the rendering viewport
     */
    void    SetViewport();

    /**
     * @brief Update performance metrics
     */
    void UpdateMetrics();

    /**
     * @brief Apply current graphics settings to D3D11 state
     */
    void ApplyGraphicsState();

    /**
     * @brief Notify console of state changes
     */
    void NotifyStateChange();

    /**
     * @brief Thread-safe metrics access helper
     * @return Current graphics metrics with thread safety
     */
    GraphicsMetrics GetMetricsThreadSafe() const;

    ComPtr<ID3D11Device1>          m_device;           ///< DirectX 11.1 device
    ComPtr<ID3D11DeviceContext1>   m_context;          ///< DirectX 11.1 device context
    ComPtr<IDXGISwapChain1>        m_swapChain;        ///< DXGI swap chain for presentation
    ComPtr<ID3D11RenderTargetView> m_renderTargetView; ///< Render target view for back buffer
    ComPtr<ID3D11DepthStencilView> m_depthStencilView; ///< Depth stencil view for depth testing

    UINT m_windowWidth;  ///< Current window width in pixels
    UINT m_windowHeight; ///< Current window height in pixels

    // Console integration state
    GraphicsSettings m_settings;              ///< Current graphics settings
    mutable std::mutex m_metricsMutex;        ///< Thread safety for metrics access
    std::function<void()> m_stateCallback;    ///< Callback for state changes
    
    // Performance tracking
    mutable GraphicsMetrics m_metrics;        ///< Current performance metrics
    std::chrono::high_resolution_clock::time_point m_frameStartTime; ///< Frame timing
    std::chrono::high_resolution_clock::time_point m_renderStartTime; ///< Render timing
    
    // D3D11 state objects for console control
    ComPtr<ID3D11RasterizerState> m_solidRasterState;     ///< Solid rendering state
    ComPtr<ID3D11RasterizerState> m_wireframeRasterState; ///< Wireframe rendering state
    ComPtr<ID3D11Query> m_gpuTimingQuery;                 ///< GPU timing query
    
    // Resource tracking
    size_t m_textureMemoryUsage;              ///< Current texture memory usage
    size_t m_bufferMemoryUsage;               ///< Current buffer memory usage
};