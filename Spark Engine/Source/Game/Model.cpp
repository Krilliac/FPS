#include "Model.h"
#include "ModelVertex.h"
#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>

HRESULT Model::LoadObj(const std::wstring& filename,
    ID3D11Device* device)
{
    // Convert wide filename to UTF-8
    std::string fileUtf8(filename.begin(), filename.end());

    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  mats;
    std::string                       warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &warn, &err, fileUtf8.c_str()))
        return E_FAIL;

    std::vector<ModelVertex> verts;
    std::vector<UINT>        idxs;
    verts.reserve(shapes.size() * 16);
    idxs.reserve(shapes.size() * 16);

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            DirectX::XMFLOAT3 pos{
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            };

            DirectX::XMFLOAT3 norm{ 0.f,0.f,0.f };
            if (idx.normal_index >= 0) {
                norm = DirectX::XMFLOAT3(
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                );
            }

            DirectX::XMFLOAT2 uv{ 0.f,0.f };
            if (idx.texcoord_index >= 0) {
                uv = DirectX::XMFLOAT2(
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    attrib.texcoords[2 * idx.texcoord_index + 1]
                );
            }

            ModelVertex v(pos, norm, uv);
            idxs.push_back(static_cast<UINT>(verts.size()));
            verts.push_back(v);
        }
    }

    m_indexCount = static_cast<UINT>(idxs.size());

    // Create vertex buffer
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = UINT(verts.size() * sizeof(ModelVertex));
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd{ verts.data(), 0, 0 };

    HRESULT hr = device->CreateBuffer(&bd, &sd, &m_vb);
    if (FAILED(hr)) return hr;

    // Create index buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = UINT(idxs.size() * sizeof(UINT));
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = idxs.data();

    hr = device->CreateBuffer(&bd, &sd, &m_ib);
    return hr;
}

void Model::Render(ID3D11DeviceContext* ctx)
{
    UINT stride = sizeof(ModelVertex);
    UINT offset = 0;
    ctx->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
    ctx->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->DrawIndexed(m_indexCount, 0, 0);
}