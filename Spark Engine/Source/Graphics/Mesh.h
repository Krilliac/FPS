// Mesh.h
#pragma once

#include "Utils/Assert.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <cmath>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;

    Vertex() : Position{ 0,0,0 }, Normal{ 0,1,0 }, TexCoord{ 0,0 } {}

    Vertex(const XMFLOAT3& p,
        const XMFLOAT3& n,
        const XMFLOAT2& t)
        : Position(p), Normal(n), TexCoord(t)
    {
        ASSERT(std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z));
        ASSERT(std::isfinite(n.x) && std::isfinite(n.y) && std::isfinite(n.z));
        ASSERT(std::isfinite(t.x) && std::isfinite(t.y));
    }
};

struct MeshData
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void    Shutdown();

    HRESULT CreateCube(float size = 1.0f);
    HRESULT CreateTriangle(float size = 1.0f);
    HRESULT CreatePlane(float width = 10.0f, float depth = 10.0f);
    HRESULT CreateSphere(float radius = 1.0f, int slices = 20, int stacks = 20);

    HRESULT CreateFromVertices(const std::vector<Vertex>& verts,
        const std::vector<unsigned int>& inds);

    bool LoadFromFile(const std::wstring& path);

    void SetPlaceholder(bool p) { m_placeholder = p; }
    bool IsPlaceholder() const { return m_placeholder; }

    void Render(ID3D11DeviceContext* ctx);
    UINT GetVertexCount() const { return m_vertexCount; }
    UINT GetIndexCount()  const { return m_indexCount; }

private:
    HRESULT CreateBuffers();
    void    CalculateNormals();

    ID3D11Buffer* m_vb{ nullptr };
    ID3D11Buffer* m_ib{ nullptr };
    ID3D11Device* m_device{ nullptr };
    ID3D11DeviceContext* m_context{ nullptr };

    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int              m_vertexCount{ 0 };
    unsigned int              m_indexCount{ 0 };
    bool                      m_placeholder{ false };
};