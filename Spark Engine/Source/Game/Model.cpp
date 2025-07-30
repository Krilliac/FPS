#include "Model.h"
#include "ModelVertex.h"
#include "Utils/Assert.h"
#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <DirectXMath.h>

using namespace DirectX;

HRESULT Model::LoadObj(const std::wstring& filename, ID3D11Device* device)
{
    // ------------------------------------------------------------------
    //  Argument validation
    // ------------------------------------------------------------------
    ASSERT_ALWAYS_MSG(!filename.empty(), "Model::LoadObj ‒ filename is empty");
    ASSERT_ALWAYS_MSG(device != nullptr, "Model::LoadObj ‒ ID3D11Device is null");

    // Convert UTF-16 filename to UTF-8 for tinyobjloader
    std::string fileUtf8(filename.begin(), filename.end());

    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> mats;
    std::string  warn, err;

    // ------------------------------------------------------------------
    //  TinyOBJ load
    // ------------------------------------------------------------------
    if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &warn, &err, fileUtf8.c_str()))
    {
        OutputDebugStringA(("tinyobj error: " + warn + err + "\n").c_str());
        return E_FAIL;
    }
    ASSERT_ALWAYS_MSG(!shapes.empty(), "OBJ file contains no shapes");

    // ------------------------------------------------------------------
    //  Build vertex / index arrays
    // ------------------------------------------------------------------
    std::vector<ModelVertex> verts;
    std::vector<UINT>        idxs;
    verts.reserve(shapes.size() * 16);
    idxs.reserve(shapes.size() * 16);

    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            XMFLOAT3 pos{
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            };

            XMFLOAT3 norm{ 0.f, 0.f, 0.f };
            if (idx.normal_index >= 0)
            {
                norm = XMFLOAT3(
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                );
            }

            XMFLOAT2 uv{ 0.f, 0.f };
            if (idx.texcoord_index >= 0)
            {
                uv = XMFLOAT2(
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    attrib.texcoords[2 * idx.texcoord_index + 1]
                );
            }

            verts.emplace_back(pos, norm, uv);
            idxs.push_back(static_cast<UINT>(verts.size() - 1));
        }
    }

    m_indexCount = static_cast<UINT>(idxs.size());
    ASSERT_ALWAYS_MSG(m_indexCount > 0, "OBJ produced zero indices");

    // ------------------------------------------------------------------
    //  Create GPU buffers
    // ------------------------------------------------------------------
    D3D11_BUFFER_DESC bd{};
    D3D11_SUBRESOURCE_DATA sd{};

    // Vertex buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = UINT(verts.size() * sizeof(ModelVertex));
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    sd.pSysMem = verts.data();

    HRESULT hr = device->CreateBuffer(&bd, &sd, &m_vb);
    ASSERT_MSG(SUCCEEDED(hr), "Failed to create vertex buffer");
    if (FAILED(hr)) return hr;

    // Index buffer
    bd.ByteWidth = UINT(idxs.size() * sizeof(UINT));
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = idxs.data();

    hr = device->CreateBuffer(&bd, &sd, &m_ib);
    ASSERT_MSG(SUCCEEDED(hr), "Failed to create index buffer");
    return hr;
}

void Model::Render(ID3D11DeviceContext* ctx)
{
    ASSERT(ctx != nullptr);
    ASSERT(m_vb != nullptr);
    ASSERT(m_ib != nullptr);
    ASSERT(m_indexCount > 0);

    UINT stride = sizeof(ModelVertex);
    UINT offset = 0;

    ctx->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
    ctx->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->DrawIndexed(m_indexCount, 0, 0);
}