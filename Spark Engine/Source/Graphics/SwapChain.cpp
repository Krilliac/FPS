#include "SwapChain.h"
#include "../Utils/Logger.h"

namespace SparkEngine {
    SwapChain::SwapChain() {}
    SwapChain::~SwapChain() { Cleanup(); }

    bool SwapChain::Initialize(ID3D11Device* device, ID3D11DeviceContext* context,
                               HWND hwnd, int width, int height) {
        DXGI_SWAP_CHAIN_DESC desc = {};
        desc.BufferCount = 2;
        desc.BufferDesc.Width = width;
        desc.BufferDesc.Height = height;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.OutputWindow = hwnd;
        desc.SampleDesc.Count = 1;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        ComPtr<IDXGIDevice> dxgiDevice;
        device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());

        ComPtr<IDXGIAdapter> adapter;
        dxgiDevice->GetAdapter(adapter.GetAddressOf());

        ComPtr<IDXGIFactory> factory;
        adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());

        HRESULT hr = factory->CreateSwapChain(device, &desc, m_swapChain.GetAddressOf());
        if (FAILED(hr)) {
            Logger::Error("Failed to create swap chain");
            return false;
        }

        // Create RTV
        ComPtr<ID3D11Texture2D> backBuffer;
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
        device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.GetAddressOf());

        // Create DSV
        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ComPtr<ID3D11Texture2D> depthBuffer;
        device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf());
        device->CreateDepthStencilView(depthBuffer.Get(), nullptr, m_dsv.GetAddressOf());

        // Bind views
        context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_dsv.Get());
        context->RSSetViewports(1, &CD3D11_VIEWPORT(0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f));

        return true;
    }

    void SwapChain::Cleanup() {
        if (m_swapChain) {
            m_swapChain->SetFullscreenState(FALSE, nullptr);
        }
    }

    void SwapChain::Present() {
        m_swapChain->Present(1, 0);
    }
}
