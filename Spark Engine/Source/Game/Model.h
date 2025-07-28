#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

struct ModelVertex {
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT2 TexCoord;
};

class Model {
public:
    HRESULT LoadObj(const std::wstring& filename,
                    ID3D11Device* device);
    void    Render(ID3D11DeviceContext* ctx);

private:
    ID3D11Buffer*        m_vb{nullptr};
    ID3D11Buffer*        m_ib{nullptr};
    UINT                 m_indexCount{0};
};
