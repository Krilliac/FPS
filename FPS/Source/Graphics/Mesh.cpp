#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <cwchar>
#include <DirectXMath.h>
#include "Mesh.h"

using namespace DirectX;

Mesh::Mesh() = default;

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
    if (m_ib) { m_ib->Release();  m_ib = nullptr; }
    if (m_vb) { m_vb->Release();  m_vb = nullptr; }
    m_vertices.clear();
    m_indices.clear();
}

bool Mesh::LoadFromFile(const std::wstring& path)
{
    // TODO: integrate with actual FBX/OBJ loader here.
    // Return true if load succeeds, false otherwise.
    return false;
}

HRESULT Mesh::CreateFromVertices(const std::vector<Vertex>& verts,
    const std::vector<unsigned int>& inds)
{
    m_vertices = verts;
    m_indices = inds;
    m_vertexCount = static_cast<unsigned int>(m_vertices.size());
    m_indexCount = static_cast<unsigned int>(m_indices.size());
    CalculateNormals();
    return CreateBuffers();
}

HRESULT Mesh::CreateCube(float size)
{
    MeshData md;
    float h = size * 0.5f;
    XMFLOAT3 pts[8] = {
        {-h,-h,-h},{+h,-h,-h},{+h,+h,-h},{-h,+h,-h},
        {-h,-h,+h},{+h,-h,+h},{+h,+h,+h},{-h,+h,+h}
    };
    XMFLOAT3 norms[6] = {
        {0,0,-1},{0,0,1},{-1,0,0},
        {1,0,0},{0,-1,0},{0,1,0}
    };
    unsigned int idxs[36] = {
        0,1,2, 0,2,3, 4,6,5, 4,7,6,
        4,5,1, 4,1,0, 3,2,6, 3,6,7,
        4,0,3, 4,3,7, 1,5,6, 1,6,2
    };
    for (int f = 0; f < 6; ++f)
    {
        XMFLOAT3 n = norms[f];
        for (int v = 0; v < 6; ++v)
        {
            unsigned int i = idxs[f * 6 + v];
            md.vertices.emplace_back(pts[i], n, XMFLOAT2(0, 0));
            md.indices.push_back(static_cast<unsigned int>(md.indices.size()));
        }
    }
    return CreateFromVertices(md.vertices, md.indices);
}

HRESULT Mesh::CreatePlane(float width, float depth)
{
    MeshData md;
    float hw = width * 0.5f, hd = depth * 0.5f;
    XMFLOAT3 pts[4] = { {-hw,0,-hd},{+hw,0,-hd},{+hw,0,+hd},{-hw,0,+hd} };
    XMFLOAT3 n = { 0,1,0 };
    unsigned int idxs[6] = { 0,1,2, 0,2,3 };
    for (int i = 0; i < 6; ++i)
    {
        md.vertices.emplace_back(pts[idxs[i]], n, XMFLOAT2(0, 0));
        md.indices.push_back(static_cast<unsigned int>(md.indices.size()));
    }
    return CreateFromVertices(md.vertices, md.indices);
}

HRESULT Mesh::CreateSphere(float radius, int slices, int stacks)
{
    MeshData md;
    for (int i = 0; i <= stacks; ++i)
    {
        float v = i / static_cast<float>(stacks);
        float phi = v * XM_PI;
        for (int j = 0; j <= slices; ++j)
        {
            float u = j / static_cast<float>(slices);
            float theta = u * XM_2PI;
            XMFLOAT3 pos = {
                radius * sinf(phi) * cosf(theta),
                radius * cosf(phi),
                radius * sinf(phi) * sinf(theta)
            };
            XMVECTOR nVec = XMVector3Normalize(XMLoadFloat3(&pos));
            XMFLOAT3 n; XMStoreFloat3(&n, nVec);
            md.vertices.emplace_back(pos, n, XMFLOAT2(u, v));
        }
    }
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int a = i * (slices + 1) + j;
            int b = a + slices + 1;
            md.indices.insert(md.indices.end(), {
                static_cast<unsigned int>(a),
                static_cast<unsigned int>(b),
                static_cast<unsigned int>(a + 1),
                static_cast<unsigned int>(b),
                static_cast<unsigned int>(b + 1),
                static_cast<unsigned int>(a + 1)
                });
        }
    }
    return CreateFromVertices(md.vertices, md.indices);
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

        XMVECTOR n = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0));
        XMFLOAT3 nf; XMStoreFloat3(&nf, n);

        m_vertices[i0].Normal = nf;
        m_vertices[i1].Normal = nf;
        m_vertices[i2].Normal = nf;
    }
}

HRESULT Mesh::CreateBuffers()
{
    if (!m_device) return E_FAIL;

    // Vertex buffer
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(Vertex) * m_vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vsd = { m_vertices.data() };
    HRESULT hr = m_device->CreateBuffer(&vbd, &vsd, &m_vb);
    if (FAILED(hr)) return hr;

    // Index buffer
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(unsigned int) * m_indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA isd = { m_indices.data() };
    return m_device->CreateBuffer(&ibd, &isd, &m_ib);
}

void Mesh::Render()
{
    if (!m_context || !m_vb || !m_ib) return;

    UINT stride = sizeof(Vertex), offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
    m_context->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->DrawIndexed(m_indexCount, 0, 0);
}