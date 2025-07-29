#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

class Model {
public:
    // Load OBJ from wide‐string filename, create D3D11 buffers
    HRESULT LoadObj(const std::wstring& filename,
        ID3D11Device* device);

    // Render previously loaded mesh
    void Render(ID3D11DeviceContext* ctx);

private:
    ID3D11Buffer* m_vb{ nullptr };
    ID3D11Buffer* m_ib{ nullptr };
    UINT          m_indexCount{ 0 };
};