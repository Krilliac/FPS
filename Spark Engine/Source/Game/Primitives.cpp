#include <Windows.h>
#include <cstdint>
#include <cmath>
#include <DirectXMath.h>
#include "Primitives.h"

using namespace DirectX;

MeshData Primitives::CreateCube(float size)
{
    MeshData m;
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
    for (int f = 0; f < 6; f++) {
        XMFLOAT3 n = norms[f];
        for (int v = 0; v < 6; v++) {
            unsigned int i = idxs[f * 6 + v];
            m.vertices.emplace_back(pts[i], n, XMFLOAT2(0, 0));
            m.indices.push_back((unsigned int)m.indices.size());
        }
    }
    return m;
}

MeshData Primitives::CreatePlane(float width, float depth)
{
    MeshData m;
    float hw = width * 0.5f, hd = depth * 0.5f;
    XMFLOAT3 pts[4] = { {-hw,0,-hd},{+hw,0,-hd},{+hw,0,+hd},{-hw,0,+hd} };
    XMFLOAT3 n = { 0,1,0 };
    unsigned int idxs[6] = { 0,1,2, 0,2,3 };
    for (int i = 0; i < 6; i++) {
        m.vertices.emplace_back(pts[idxs[i]], n, XMFLOAT2(0, 0));
        m.indices.push_back((unsigned int)m.indices.size());
    }
    return m;
}

MeshData Primitives::CreateSphere(float radius, int slices, int stacks)
{
    MeshData m;
    for (int i = 0; i <= stacks; i++) {
        float v = i / (float)stacks, phi = v * XM_PI;
        for (int j = 0; j <= slices; j++) {
            float u = j / (float)slices, theta = u * XM_2PI;
            XMFLOAT3 pos = {
                radius * sinf(phi) * cosf(theta),
                radius * cosf(phi),
                radius * sinf(phi) * sinf(theta)
            };
            XMVECTOR vn = XMVector3Normalize(XMLoadFloat3(&pos));
            XMFLOAT3 norm; XMStoreFloat3(&norm, vn);
            m.vertices.emplace_back(pos, norm, XMFLOAT2(u, v));
        }
    }
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            int a = i * (slices + 1) + j;
            int b = a + slices + 1;
            m.indices.insert(m.indices.end(), {
                (unsigned int)a,(unsigned int)b,(unsigned int)(a + 1),
                (unsigned int)b,(unsigned int)(b + 1),(unsigned int)(a + 1)
                });
        }
    }
    return m;
}
