// Mesh.cpp
#include "Mesh.h"
#include "Utils/Assert.h"
//#include "Utils/Debug.h"
#include <tiny_obj_loader.h>
#include <DirectXMath.h>
#include <fstream>
#include <filesystem>   // C++17 for path handling
#include <cmath>
#include <iostream>

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
    auto u8path_u8 = std::filesystem::path(path).u8string();                                    // basic_string<char8_t>
    std::string u8Path(u8path_u8.begin(), u8path_u8.end());                                     // convert to std::string

    // Debug: report attempt
    std::wcerr << L"[DEBUG] Attempting to load mesh from: " << path << std::endl;

    // Reader config
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig cfg;
    {
        auto parent_u8 = std::filesystem::path(path).parent_path().u8string();
        std::string mtlSearch(parent_u8.begin(), parent_u8.end());
        cfg.mtl_search_path = mtlSearch;                                                         // now a std::string
    }

    // Replace the fallback section in LoadFromFile with this:
    if (!reader.ParseFromFile(u8Path, cfg))
    {
        // Report tinyobj error
        if (!reader.Error().empty())
        {
            std::string err = "tinyobj error: " + reader.Error() + "\n";
            OutputDebugStringA(err.c_str());
            std::cerr << err;
        }

        // Do NOT create placeholder here - let the calling code handle it
        std::wcerr << L"[DEBUG] Failed to load mesh from file." << std::endl;
        return false; // Return false so placeholder creation happens in calling code
    }

    // Report tinyobj warnings
    if (!reader.Warning().empty())
    {
        std::string warn = "tinyobj warning: " + reader.Warning() + "\n";
        OutputDebugStringA(warn.c_str());
        std::cerr << warn;
    }

    // Parse geometry
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;
    verts.reserve(attrib.vertices.size() / 3);

    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            Vertex v{};
            // Position
            v.Position = {
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            };
            // Normal
            if (idx.normal_index >= 0)
            {
                v.Normal = {
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                };
            }
            else
            {
                v.Normal = { 0, 1, 0 };
            }
            // TexCoord
            if (idx.texcoord_index >= 0)
            {
                v.TexCoord = {
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                };
            }
            else
            {
                v.TexCoord = { 0, 0 };
            }

            verts.push_back(v);
            inds.push_back(static_cast<unsigned int>(inds.size()));
        }
    }

    // Ensure we got geometry
    ASSERT_ALWAYS_MSG(!verts.empty() && !inds.empty(), "tinyobj produced empty mesh data");

    m_vertices = std::move(verts);
    m_indices = std::move(inds);
    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());

    // Recompute normals if any were zero
    bool anyZero = std::any_of(
        m_vertices.begin(), m_vertices.end(),
        [](const Vertex& v) {
            return v.Normal.x == 0 && v.Normal.y == 0 && v.Normal.z == 0;
        });
    if (anyZero)
        CalculateNormals();

    // Create GPU buffers
    HRESULT hr = CreateBuffers();
    if (FAILED(hr))
    {
        std::wcerr << L"[ERROR] CreateBuffers failed in LoadFromFile!" << std::endl;
        return false;
    }

    std::wcerr << L"[DEBUG] Successfully loaded mesh: " << path << std::endl;
    return true;
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

    std::wcerr << L"[DEBUG] CreateCube called with size: " << size << std::endl;

    // Clear existing data
    m_vertices.clear();
    m_indices.clear();

    float h = size * 0.5f;

    // 24 vertices (4 per face, 6 faces)
    m_vertices = {
        // Front face (z = +h)
        {{-h, -h,  h}, {0, 0, 1}, {0, 1}},  // 0
        {{ h, -h,  h}, {0, 0, 1}, {1, 1}},  // 1
        {{ h,  h,  h}, {0, 0, 1}, {1, 0}},  // 2
        {{-h,  h,  h}, {0, 0, 1}, {0, 0}},  // 3

        // Back face (z = -h)  
        {{ h, -h, -h}, {0, 0, -1}, {0, 1}}, // 4
        {{-h, -h, -h}, {0, 0, -1}, {1, 1}}, // 5
        {{-h,  h, -h}, {0, 0, -1}, {1, 0}}, // 6
        {{ h,  h, -h}, {0, 0, -1}, {0, 0}}, // 7

        // Left face (x = -h)
        {{-h, -h, -h}, {-1, 0, 0}, {0, 1}}, // 8
        {{-h, -h,  h}, {-1, 0, 0}, {1, 1}}, // 9
        {{-h,  h,  h}, {-1, 0, 0}, {1, 0}}, // 10
        {{-h,  h, -h}, {-1, 0, 0}, {0, 0}}, // 11

        // Right face (x = +h)
        {{ h, -h,  h}, {1, 0, 0}, {0, 1}},  // 12
        {{ h, -h, -h}, {1, 0, 0}, {1, 1}},  // 13
        {{ h,  h, -h}, {1, 0, 0}, {1, 0}},  // 14
        {{ h,  h,  h}, {1, 0, 0}, {0, 0}},  // 15

        // Bottom face (y = -h)
        {{-h, -h, -h}, {0, -1, 0}, {0, 1}}, // 16
        {{ h, -h, -h}, {0, -1, 0}, {1, 1}}, // 17
        {{ h, -h,  h}, {0, -1, 0}, {1, 0}}, // 18
        {{-h, -h,  h}, {0, -1, 0}, {0, 0}}, // 19

        // Top face (y = +h)
        {{-h,  h,  h}, {0, 1, 0}, {0, 1}},  // 20
        {{ h,  h,  h}, {0, 1, 0}, {1, 1}},  // 21
        {{ h,  h, -h}, {0, 1, 0}, {1, 0}},  // 22
        {{-h,  h, -h}, {0, 1, 0}, {0, 0}}   // 23
    };

    // 36 indices (2 triangles per face, 6 faces)
    m_indices = {
        // Front face
        0, 1, 2,  0, 2, 3,
        // Back face
        4, 5, 6,  4, 6, 7,
        // Left face  
        8, 9, 10,  8, 10, 11,
        // Right face
        12, 13, 14,  12, 14, 15,
        // Bottom face
        16, 17, 18,  16, 18, 19,
        // Top face
        20, 21, 22,  20, 22, 23
    };

    m_vertexCount = static_cast<UINT>(m_vertices.size());
    m_indexCount = static_cast<UINT>(m_indices.size());

    std::wcerr << L"[DEBUG] Created cube with " << m_vertexCount << L" vertices and "
        << m_indexCount << L" indices" << std::endl;

    ASSERT_ALWAYS_MSG(m_vertexCount > 0 && m_indexCount > 0, "CreateCube produced empty mesh");

    // Create GPU buffers
    HRESULT hr = CreateBuffers();
    if (FAILED(hr)) {
        std::wcerr << L"[ERROR] CreateBuffers failed in CreateCube!" << std::endl;
        return hr;
    }

    std::wcerr << L"[DEBUG] CreateCube completed successfully" << std::endl;
    return S_OK;
}

HRESULT Mesh::CreateTriangle(float size)
{
    ASSERT(size > 0.0f);

    std::wcerr << L"[DEBUG] CreateTriangle called with size: " << size << std::endl;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float h = size * 0.5f;

    // Create a simple triangle
    vertices.emplace_back(
        XMFLOAT3(0.0f, h, 0.0f),      // Top vertex
        XMFLOAT3(0.0f, 0.0f, 1.0f),   // Normal pointing forward
        XMFLOAT2(0.5f, 0.0f)          // Texture coordinate
    );

    vertices.emplace_back(
        XMFLOAT3(-h, -h, 0.0f),       // Bottom left
        XMFLOAT3(0.0f, 0.0f, 1.0f),   // Normal pointing forward
        XMFLOAT2(0.0f, 1.0f)          // Texture coordinate
    );

    vertices.emplace_back(
        XMFLOAT3(h, -h, 0.0f),        // Bottom right
        XMFLOAT3(0.0f, 0.0f, 1.0f),   // Normal pointing forward
        XMFLOAT2(1.0f, 1.0f)          // Texture coordinate
    );

    // Triangle indices
    indices = { 0, 1, 2 };

    std::wcerr << L"[DEBUG] Created triangle with " << vertices.size() << L" vertices and "
        << indices.size() << L" indices" << std::endl;

    ASSERT_ALWAYS_MSG(!vertices.empty() && !indices.empty(), "CreateTriangle produced empty mesh");

    return CreateFromVertices(vertices, indices);
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