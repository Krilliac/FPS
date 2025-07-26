#pragma once
#include "..\Core\framework.h"

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
    
    Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 0.0f) {}
    Vertex(const XMFLOAT3& pos, const XMFLOAT3& norm, const XMFLOAT2& tex)
        : Position(pos), Normal(norm), TexCoord(tex) {}
};

class Mesh
{
private:
    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    
    std::vector<Vertex> m_vertices;
    std::vector<UINT> m_indices;
    
    UINT m_vertexCount;
    UINT m_indexCount;

public:
    Mesh();
    ~Mesh();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    
    // Mesh creation methods
    HRESULT CreateCube(float size = 1.0f);
    HRESULT CreatePlane(float width = 10.0f, float depth = 10.0f);
    HRESULT CreateSphere(float radius = 1.0f, int slices = 20, int stacks = 20);
    
    // Custom mesh creation
    HRESULT CreateFromVertices(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
    
    // Rendering
    void Render();
    
    // Accessors
    UINT GetVertexCount() const { return m_vertexCount; }
    UINT GetIndexCount() const { return m_indexCount; }

private:
    HRESULT CreateBuffers();
    void CalculateNormals();
};
