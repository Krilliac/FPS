#include "D3DUtils.h"

// Define the globals
IDXGISwapChain* g_MainSwapChain = nullptr;
ID3D11Device* g_D3DDevice = nullptr;
ID3D11DeviceContext* g_D3DContext = nullptr;

// Implement the accessors
IDXGISwapChain* GetMainSwapChain() { return g_MainSwapChain; }
ID3D11Device* GetD3DDevice() { return g_D3DDevice; }
ID3D11DeviceContext* GetD3DContext() { return g_D3DContext; }