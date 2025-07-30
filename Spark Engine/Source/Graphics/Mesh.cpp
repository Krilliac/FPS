// Mesh.cpp
#include "Mesh.h"
#include "Utils/Assert.h"
//#include "Utils/Debug.h"
#include <tiny_obj_loader.h>
#include <DirectXMath.h>
#include <fstream>
#include <filesystem>   // C++17 for path handling
#include <cmath>

using namespace DirectX;

Mesh::Mesh() = default;
Mesh::~Mesh() { Shutdown(); }

HRESULT Mesh::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT(device && context);
    m_device = device;
    m_context = context;
    return S_OK;
}

void Mesh::Shutdown()
{
    if (m_ib) { m_ib->Release(); m_ib = nullptr; }
    if (m_vb) { m_vb->Release(); m_vb = nullptr; }
    m_vertices.clear();
    m_indices.clear();
    m_vertexCount = m_indexCount = 0;
    m_device = nullptr;
    m_context = nullptr;
}

bool Mesh::LoadFromFile(const std::wstring& path)
{
    ASSERT_ALWAYS_MSG(!path.empty(), "Mesh::LoadFromFile – empty path");

    // Convert wide path to UTF-8 for tinyobjloader
    std::string u8Path = std::filesystem::path(path).u8string();

    tinyobj::ObjReader       reader;
    tinyobj::ObjReaderConfig cfg;
    cfg.mtl_search_path = std::filesystem::path(path).parent_path().u8string();

    if (!reader.ParseFromFile(u8Path, cfg))
    {
        if (!reader.Error().empty())
            OutputDebugStringA(("tinyobj error: " + reader.Error() + '\n').c_str());
        return false;
    }
    if (!reader.Warning().empty())
        OutputDebugStringA(("tinyobj warning: " + reader.Warning() + '\n').c_str());

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();

    std::vector<Vertex>       verts;
    std::vector<unsigned int> inds;
    verts.reserve(attrib.vertices.size() / 3);

    for (const auto& sh : shapes)
    {
        for (const auto& idx : sh.mesh.indices)
        {
            XMFLOAT3 pos{
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            };

            XMFLOAT3 nrm{ 0, 1, 0 };
            if (idx.normal_index >= 0)
            {
                nrm = {
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                };
            }

            XMFLOAT2 uv{ 0, 0 };
            if (idx.texcoord_index >= 0)
            {
                uv = {
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                };
            }

            verts.emplace_back(pos, nrm, uv);
            inds.push_back(static_cast<unsigned int>(inds.size()));
        }
    }

    ASSERT_ALWAYS_MSG(!verts.empty() && !inds.empty(), "tinyobj produced empty mesh data");

    m_vertices = std::move(verts);
    m_indices = std::move(inds);
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());

    // Recompute normals if any came in as zero
    bool anyZeroNormal = std::any_of(
        m_vertices.begin(), m_vertices.end(),
        [](const Vertex& v) { return v.Normal.x == 0 && v.Normal.y == 0 && v.Normal.z == 0; });

    if (anyZeroNormal)
        CalculateNormals();

    return SUCCEEDED(CreateBuffers());
}

HRESULT Mesh::CreateFromVertices(const std::vector<Vertex>& verts,
    const std::vector<unsigned int>& inds)
{
    ASSERT(!verts.empty() && !inds.empty());

    m_vertices = verts;
    m_indices = inds;
    m_vertexCount = static_cast<UINT>(verts.size());
    m_indexCount = static_cast<UINT>(inds.size());

    CalculateNormals();
    return CreateBuffers();
}

HRESULT Mesh::CreateCube(float size)
{
    ASSERT(size > 0.0f);

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

    ASSERT_ALWAYS_MSG(!md.vertices.empty() && !md.indices.empty(),
        "CreateCube produced empty mesh");

    return CreateFromVertices(md.vertices, md.indices);
}

HRESULT Mesh::CreatePlane(float width, float depth)
{
    ASSERT(width > 0.0f && depth > 0.0f);

    MeshData md;
    float hw = width * 0.5f, hd = depth * 0.5f;
    XMFLOAT3 pts[4] = { {-hw,0,-hd},{+hw,0,-hd},{+hw,0,+hd},{-hw,0,+hd} };
    XMFLOAT3 n{ 0,1,0 };
    unsigned int idxs[6] = { 0,1,2, 0,2,3 };

    for (int i = 0; i < 6; ++i)
    {
        md.vertices.emplace_back(pts[idxs[i]], n, XMFLOAT2(0, 0));
        md.indices.push_back(static_cast<unsigned int>(md.indices.size()));
    }

    ASSERT_ALWAYS_MSG(!md.vertices.empty() && !md.indices.empty(),
        "CreatePlane produced empty mesh");

    return CreateFromVertices(md.vertices, md.indices);
}

HRESULT Mesh::CreateSphere(float radius, int slices, int stacks)
{
    ASSERT(radius > 0.0f);
    ASSERT(slices >= 3 && stacks >= 2);

    MeshData md;
    for (int i = 0; i <= stacks; ++i)
    {
        float v = i / static_cast<float>(stacks);
        float phi = v * XM_PI;
        for (int j = 0; j <= slices; ++j)
        {
            float u = j / static_cast<float>(slices);
            float theta = u * XM_2PI;

            XMFLOAT3 pos{
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

    ASSERT_ALWAYS_MSG(!md.vertices.empty() && !md.indices.empty(),
        "CreateSphere produced empty mesh");

    return CreateFromVertices(md.vertices, md.indices);
}

void Mesh::CalculateNormals()
{
    ASSERT(!m_vertices.empty() && !m_indices.empty());

    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        unsigned i0 = m_indices[i], i1 = m_indices[i + 1], i2 = m_indices[i + 2];
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
    ASSERT(m_device);
    ASSERT(!m_vertices.empty() && !m_indices.empty());

    // Vertex buffer
    D3D11_BUFFER_DESC vbd{};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = static_cast<UINT>(m_vertices.size() * sizeof(Vertex));
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vsd{ m_vertices.data(), 0, 0 };

    HRESULT hr = m_device->CreateBuffer(&vbd, &vsd, &m_vb);
    ASSERT_MSG(SUCCEEDED(hr), "CreateBuffer (VB) failed");
    if (FAILED(hr)) return hr;

    // Index buffer
    D3D11_BUFFER_DESC ibd{};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = static_cast<UINT>(m_indices.size() * sizeof(unsigned int));
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA isd{};
    isd.pSysMem = m_indices.data();

    hr = m_device->CreateBuffer(&ibd, &isd, &m_ib);
    ASSERT_MSG(SUCCEEDED(hr), "CreateBuffer (IB) failed");
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Mesh::Render(ID3D11DeviceContext* ctx)
{
    ASSERT(ctx && m_vb && m_ib && m_indexCount > 0);

    UINT stride = sizeof(Vertex), offset = 0;
    ctx->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
    ctx->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->DrawIndexed(m_indexCount, 0, 0);
}