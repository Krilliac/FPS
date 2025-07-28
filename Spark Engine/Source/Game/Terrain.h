#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;

struct TerrainVertex {
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
};

class Terrain {
public:
    // Load heightmap from 8-bit BMP; build vertex/index buffers
    HRESULT Initialize(ID3D11Device* device,
                       ID3D11DeviceContext* ctx,
                       const wchar_t* heightmapFile,
                       UINT width, UINT height,
                       float cellSpacing);
    void Render(ID3D11DeviceContext* ctx);

private:
    void BuildMesh(const std::vector<uint8_t>& heightData,
                   UINT w, UINT h, float s);
    void CalculateNormals();

    ID3D11Buffer*            m_vb{nullptr};
    ID3D11Buffer*            m_ib{nullptr};
    UINT                     m_indexCount{0};
    std::vector<TerrainVertex> m_vertices;
    std::vector<UINT>        m_indices;
};
