#include "Mesh.h"

Mesh::Mesh()
    : m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_device(nullptr)
    , m_context(nullptr)
    , m_vertexCount(0)
    , m_indexCount(0)
{
}

Mesh::~Mesh()
{
    Shutdown();
}

HRESULT Mesh::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    return S_OK;
}

void Mesh::Shutdown()
{
    if (m_indexBuffer) { m_indexBuffer->Release(); m_indexBuffer = nullptr; }
    if (m_vertexBuffer) { m_vertexBuffer->Release(); m_vertexBuffer = nullptr; }
}

HRESULT Mesh::CreateCube(float size)
{
    float halfSize = size * 0.5f;
    
    m_vertices.clear();
    m_indices.clear();
    
    // Define cube vertices
    m_vertices = {
        // Front face
        Vertex(XMFLOAT3(-halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3(-halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)),
        
        // Back face
        Vertex(XMFLOAT3( halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3(-halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3(-halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
        
        // Left face
        Vertex(XMFLOAT3(-halfSize, -halfSize,  halfSize), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3(-halfSize,  halfSize,  halfSize), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3(-halfSize,  halfSize, -halfSize), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3(-halfSize, -halfSize, -halfSize), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)),
        
        // Right face
        Vertex(XMFLOAT3( halfSize, -halfSize, -halfSize), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize, -halfSize), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize,  halfSize), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize, -halfSize,  halfSize), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)),
        
        // Top face
        Vertex(XMFLOAT3(-halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3(-halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)),
        
        // Bottom face
        Vertex(XMFLOAT3(-halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3(-halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3( halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))
    };
    
    // Define cube indices
    m_indices = {
        0,1,2,    0,2,3,    // Front face
        4,5,6,    4,6,7,    // Back face
        8,9,10,   8,10,11,  // Left face
        12,13,14, 12,14,15, // Right face
        16,17,18, 16,18,19, // Top face
        20,21,22, 20,22,23  // Bottom face
    };
    
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());
    
    return CreateBuffers();
}

HRESULT Mesh::CreatePlane(float width, float depth)
{
    float halfWidth = width * 0.5f;
    float halfDepth = depth * 0.5f;
    
    m_vertices.clear();
    m_indices.clear();
    
    // Create plane vertices
    m_vertices = {
        Vertex(XMFLOAT3(-halfWidth, 0.0f, -halfDepth), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        Vertex(XMFLOAT3(-halfWidth, 0.0f,  halfDepth), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        Vertex(XMFLOAT3( halfWidth, 0.0f,  halfDepth), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        Vertex(XMFLOAT3( halfWidth, 0.0f, -halfDepth), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))
    };
    
    m_indices = { 0, 1, 2, 0, 2, 3 };
    
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());
    
    return CreateBuffers();
}

HRESULT Mesh::CreateFromVertices(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
    m_vertices = vertices;
    m_indices = indices;
    
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());
    
    return CreateBuffers();
}

void Mesh::Render()
{
    if (!m_vertexBuffer || !m_indexBuffer) return;
    
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    
    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->DrawIndexed(m_indexCount, 0, 0);
}

HRESULT Mesh::CreateBuffers()
{
    if (!m_device) return E_FAIL;
    
    // Create vertex buffer
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(Vertex) * m_vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA vInitData = {};
    vInitData.pSysMem = m_vertices.data();
    
    HRESULT hr = m_device->CreateBuffer(&vbd, &vInitData, &m_vertexBuffer);
    if (FAILED(hr)) return hr;
    
    // Create index buffer
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(UINT) * m_indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA iInitData = {};
    iInitData.pSysMem = m_indices.data();
    
    return m_device->CreateBuffer(&ibd, &iInitData, &m_indexBuffer);
}

HRESULT Mesh::CreateSphere(float radius, int slices, int stacks)
{
    m_vertices.clear();
    m_indices.clear();
    
    // Generate sphere vertices
    for (int i = 0; i <= stacks; ++i)
    {
        float V = i / (float)stacks;
        float phi = V * DirectX::XM_PI;
        
        for (int j = 0; j <= slices; ++j)
        {
            float U = j / (float)slices;
            float theta = U * (DirectX::XM_PI * 2);
            
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);
            
            m_vertices.push_back(Vertex(
                XMFLOAT3(x * radius, y * radius, z * radius),
                XMFLOAT3(x, y, z),
                XMFLOAT2(U, V)
            ));
        }
    }
    
    // Generate sphere indices
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int a = i * (slices + 1) + j;
            int b = a + slices + 1;
            
            m_indices.push_back(a);
            m_indices.push_back(b);
            m_indices.push_back(a + 1);
            
            m_indices.push_back(b);
            m_indices.push_back(b + 1);
            m_indices.push_back(a + 1);
        }
    }
    
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());
    
    return CreateBuffers();
}

void Mesh::CalculateNormals()
{
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        UINT i0 = m_indices[i];
        UINT i1 = m_indices[i + 1];
        UINT i2 = m_indices[i + 2];
        
        XMVECTOR v0 = XMLoadFloat3(&m_vertices[i0].Position);
        XMVECTOR v1 = XMLoadFloat3(&m_vertices[i1].Position);
        XMVECTOR v2 = XMLoadFloat3(&m_vertices[i2].Position);
        
        XMVECTOR e1 = v1 - v0;
        XMVECTOR e2 = v2 - v0;
        XMVECTOR normal = XMVector3Cross(e1, e2);
        normal = XMVector3Normalize(normal);
        
        XMStoreFloat3(&m_vertices[i0].Normal, normal);
        XMStoreFloat3(&m_vertices[i1].Normal, normal);
        XMStoreFloat3(&m_vertices[i2].Normal, normal);
    }
}
