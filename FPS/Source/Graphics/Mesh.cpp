#include "Mesh.h"
#include <cmath>
#include <DirectXMath.h>
using namespace DirectX;

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
    if (m_indexBuffer) { m_indexBuffer->Release();  m_indexBuffer = nullptr; }
    if (m_vertexBuffer) { m_vertexBuffer->Release(); m_vertexBuffer = nullptr; }
    m_vertices.clear();
    m_indices.clear();
}

HRESULT Mesh::CreateFromVertices(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    m_vertices = vertices;
    m_indices = indices;
    m_vertexCount = static_cast<unsigned int>(m_vertices.size());
    m_indexCount = static_cast<unsigned int>(m_indices.size());
    CalculateNormals();
    return CreateBuffers();
}

HRESULT Mesh::CreateCube(float size)
{
    float h = size * 0.5f;
    m_vertices.clear();
    m_indices.clear();

    // 8 corners
    XMFLOAT3 pts[8] = {
        {-h,-h,-h},{+h,-h,-h},{+h,+h,-h},{-h,+h,-h},
        {-h,-h,+h},{+h,-h,+h},{+h,+h,+h},{-h,+h,+h}
    };

    // Face normals
    XMFLOAT3 norms[6] = {
        {0,0,-1},{0,0,1},{-1,0,0},
        {1,0,0},{0,-1,0},{0,1,0}
    };

    unsigned int faceIdxs[36] = {
        0,1,2, 0,2,3,  // back
        4,6,5, 4,7,6,  // front
        4,5,1, 4,1,0,  // bottom
        3,2,6, 3,6,7,  // top
        4,0,3, 4,3,7,  // left
        1,5,6, 1,6,2   // right
    };

    for (int f = 0; f < 6; ++f)
    {
        XMFLOAT3 n = norms[f];
        for (int v = 0; v < 6; ++v)
        {
            unsigned int idx = faceIdxs[f * 6 + v];
            XMFLOAT2 uv = { (v % 3 == 1) ? 1.0f : 0.0f, (v / 3 == 0) ? 1.0f : 0.0f };
            m_vertices.emplace_back(pts[idx], n, uv);
            m_indices.push_back(static_cast<unsigned int>(m_indices.size()));
        }
    }

    m_vertexCount = static_cast<unsigned int>(m_vertices.size());
    m_indexCount = static_cast<unsigned int>(m_indices.size());
    return CreateBuffers();
}

HRESULT Mesh::CreatePlane(float width, float depth)
{
    float hw = width * 0.5f;
    float hd = depth * 0.5f;
    m_vertices.clear();
    m_indices.clear();

    // 4 corners
    XMFLOAT3 p[4] = {
        {-hw,0,-hd},{+hw,0,-hd},{+hw,0,+hd},{-hw,0,+hd}
    };
    XMFLOAT3 n{ 0,1,0 };

    m_vertices.emplace_back(p[0], n, XMFLOAT2(0, 1));
    m_vertices.emplace_back(p[1], n, XMFLOAT2(1, 1));
    m_vertices.emplace_back(p[2], n, XMFLOAT2(1, 0));
    m_vertices.emplace_back(p[3], n, XMFLOAT2(0, 0));

    unsigned int idxs[6] = { 0,1,2,0,2,3 };
    for (unsigned int i : idxs)
        m_indices.push_back(i);

    m_vertexCount = static_cast<unsigned int>(m_vertices.size());
    m_indexCount = static_cast<unsigned int>(m_indices.size());
    return CreateBuffers();
}

HRESULT Mesh::CreateSphere(float radius, int slices, int stacks)
{
    m_vertices.clear();
    m_indices.clear();

    for (int i = 0; i <= stacks; ++i)
    {
        float v = i / (float)stacks;
        float phi = v * XM_PI;
        for (int j = 0; j <= slices; ++j)
        {
            float u = j / (float)slices;
            float theta = u * XM_2PI;
            XMFLOAT3 pos{
                radius * sinf(phi) * cosf(theta),
                radius * cosf(phi),
                radius * sinf(phi) * sinf(theta)
            };
            XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&pos));
            XMFLOAT3 norm; XMStoreFloat3(&norm, n);
            m_vertices.emplace_back(pos, norm, XMFLOAT2(u, v));
        }
    }

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

    m_vertexCount = static_cast<unsigned int>(m_vertices.size());
    m_indexCount = static_cast<unsigned int>(m_indices.size());
    return CreateBuffers();
}

void Mesh::CalculateNormals()
{
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        unsigned int i0 = m_indices[i];
        unsigned int i1 = m_indices[i + 1];
        unsigned int i2 = m_indices[i + 2];

        XMVECTOR v0 = XMLoadFloat3(&m_vertices[i0].Position);
        XMVECTOR v1 = XMLoadFloat3(&m_vertices[i1].Position);
        XMVECTOR v2 = XMLoadFloat3(&m_vertices[i2].Position);

        XMVECTOR edge1 = v1 - v0;
        XMVECTOR edge2 = v2 - v0;
        XMVECTOR n = XMVector3Normalize(XMVector3Cross(edge1, edge2));

        XMFLOAT3 norm;
        XMStoreFloat3(&norm, n);
        m_vertices[i0].Normal = norm;
        m_vertices[i1].Normal = norm;
        m_vertices[i2].Normal = norm;
    }
}

HRESULT Mesh::CreateBuffers()
{
    if (!m_device || m_vertices.empty() || m_indices.empty())
        return E_FAIL;

    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(Vertex) * m_vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vinit = {};
    vinit.pSysMem = m_vertices.data();
    HRESULT hr = m_device->CreateBuffer(&vbd, &vinit, &m_vertexBuffer);
    if (FAILED(hr)) return hr;

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(unsigned int) * m_indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA iinit = {};
    iinit.pSysMem = m_indices.data();
    hr = m_device->CreateBuffer(&ibd, &iinit, &m_indexBuffer);
    return hr;
}

void Mesh::Render()
{
    if (!m_context || !m_vertexBuffer || !m_indexBuffer) return;

    UINT stride = sizeof(Vertex), offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->DrawIndexed(m_indexCount, 0, 0);
}