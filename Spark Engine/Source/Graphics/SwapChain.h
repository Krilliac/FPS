#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class SwapChain {
    public:
        SwapChain();
        ~SwapChain();

        bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context,
                        HWND hwnd, int width, int height);
        void Cleanup();
        void Present();

        ID3D11RenderTargetView* GetRenderTargetView() const { return m_rtv.Get(); }
        ID3D11DepthStencilView*  GetDepthStencilView() const { return m_dsv.Get(); }

    private:
        ComPtr<IDXGISwapChain>           m_swapChain;
        ComPtr<ID3D11RenderTargetView>   m_rtv;
        ComPtr<ID3D11DepthStencilView>   m_dsv;
    };
}
