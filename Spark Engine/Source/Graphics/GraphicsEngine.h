/**
 * @file GraphicsEngine.h
 * @brief DirectX 11 graphics engine core implementation
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class manages the entire DirectX 11 rendering pipeline including device creation,
 * swap chain management, render target setup, depth buffer configuration, and frame
 * rendering operations. It serves as the central graphics system for the Spark Engine.
 */

#pragma once

#include "Utils/Assert.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <dxgi1_3.h>     // IDXGIFactory2, IDXGISwapChain1
#include <dxgidebug.h>   // DXGIGetDebugInterface1, IDXGIInfoQueue
#include "..\Core\framework.h"  // XMFLOAT3, XMMATRIX, HRESULT

using Microsoft::WRL::ComPtr;

/**
 * @brief Core DirectX 11 graphics engine class
 * 
 * The GraphicsEngine class encapsulates all DirectX 11 functionality required for
 * rendering 3D graphics in the Spark Engine. It manages device initialization,
 * resource creation, and provides a clean interface for rendering operations.
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

    ComPtr<ID3D11Device1>          m_device;           ///< DirectX 11.1 device
    ComPtr<ID3D11DeviceContext1>   m_context;          ///< DirectX 11.1 device context
    ComPtr<IDXGISwapChain1>        m_swapChain;        ///< DXGI swap chain for presentation
    ComPtr<ID3D11RenderTargetView> m_renderTargetView; ///< Render target view for back buffer
    ComPtr<ID3D11DepthStencilView> m_depthStencilView; ///< Depth stencil view for depth testing

    UINT m_windowWidth;  ///< Current window width in pixels
    UINT m_windowHeight; ///< Current window height in pixels
};