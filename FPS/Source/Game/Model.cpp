#include "Model.h"
#include "tiny_obj_loader.h"

HRESULT Model::LoadObj(const std::wstring& filename,
                       ID3D11Device* device)
{
    std::string file = std::string(
        filename.begin(), filename.end());
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> mats;
    std::string warn, err;
    if (!tinyobj::LoadObj(&attrib, &shapes,&mats,
                          &warn,&err,file.c_str()))
        return E_FAIL;

    std::vector<ModelVertex> verts;
    std::vector<UINT>        idxs;
    for (auto& shape : shapes) {
        for (auto& idx : shape.mesh.indices) {
            Vert v = {};
            v.Position = {
                attrib.vertices[3*idx.vertex_index+0],
                attrib.vertices[3*idx.vertex_index+1],
                attrib.vertices[3*idx.vertex_index+2]
            };
            if (idx.normal_index>=0)
                v.Normal = {
                    attrib.normals[3*idx.normal_index+0],
                    attrib.normals[3*idx.normal_index+1],
                    attrib.normals[3*idx.normal_index+2]
                };
            if (idx.texcoord_index>=0)
                v.TexCoord = {
                    attrib.texcoords[2*idx.texcoord_index+0],
                    attrib.texcoords[2*idx.texcoord_index+1]
                };
            idxs.push_back((UINT)verts.size());
            verts.push_back(v);
        }
    }
    m_indexCount = UINT(idxs.size());

    // Create buffers
    D3D11_BUFFER_DESC bd{};
    D3D11_SUBRESOURCE_DATA sd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = UINT(verts.size()*sizeof(ModelVertex));
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    sd.pSysMem = verts.data();
    HRESULT hr = device->CreateBuffer(&bd,&sd,&m_vb);
    if (FAILED(hr)) return hr;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = UINT(idxs.size()*sizeof(UINT));
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = idxs.data();
    hr = device->CreateBuffer(&bd,&sd,&m_ib);
    return hr;
}

void Model::Render(ID3D11DeviceContext* ctx)
{
    UINT stride = sizeof(ModelVertex), offset = 0;
    ctx->IASetVertexBuffers(0,1,&m_vb,&stride,&offset);
    ctx->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->DrawIndexed(m_indexCount, 0, 0);
}