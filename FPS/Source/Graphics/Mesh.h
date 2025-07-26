#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;

    Vertex()
        : Position(0, 0, 0)
        , Normal(0, 1, 0)
        , TexCoord(0, 0)
    {
    }
    Vertex(const XMFLOAT3& pos, const XMFLOAT3& norm, const XMFLOAT2& tex)
        : Position(pos)
        , Normal(norm)
        , TexCoord(tex)
    {
    }
};

struct MeshData
{
    std::vector<Vertex> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<unsigned int> indices;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    // Initialize with device/context
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void    Shutdown();

    // Procedural primitives
    HRESULT CreateCube(float size = 1.0f);
    HRESULT CreatePlane(float width = 10.0f, float depth = 10.0f);
    HRESULT CreateSphere(float radius = 1.0f, int slices = 20, int stacks = 20);

    // Custom mesh from CPU-side arrays
    HRESULT CreateFromVertices(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);

    // Render call
    void Render();

    // Accessors
    unsigned int GetVertexCount() const { return m_vertexCount; }
    unsigned int GetIndexCount()  const { return m_indexCount; }

private:
    // Buffer creation
    HRESULT CreateBuffers();

    // Compute per-face normals if needed
    void CalculateNormals();

    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    std::vector<Vertex>        m_vertices;
    std::vector<unsigned int>  m_indices;

    unsigned int m_vertexCount;
    unsigned int m_indexCount;
};