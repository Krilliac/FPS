#include "Primitives.h"
#include <cmath>

using namespace DirectX;

MeshData Primitives::CreateCube(float size)
{
    float h = size * 0.5f;
    MeshData m;
    // 8 corners
    XMFLOAT3 pts[8] = {
        {-h,-h,-h},{+h,-h,-h},{+h,+h,-h},{-h,+h,-h},
        {-h,-h,+h},{+h,-h,+h},{+h,+h,+h},{-h,+h,+h},
    };
    // normals per face
    XMFLOAT3 norms[6] = {
        { 0, 0,-1},{ 0, 0, 1},{-1, 0, 0},
        { 1, 0, 0},{ 0,-1, 0},{ 0, 1, 0}
    };
    // 6 faces, 2 triangles each
    uint32_t idxs[] = {
        0,1,2, 0,2,3,  // back
        4,6,5, 4,7,6,  // front
        4,5,1, 4,1,0,  // bottom
        3,2,6, 3,6,7,  // top
        4,0,3, 4,3,7,  // left
        1,5,6, 1,6,2   // right
    };
    for (int i=0;i<36;i+=3)
    {
        uint32_t a=idxs[i], b=idxs[i+1], c=idxs[i+2];
        XMFLOAT3 n = norms[i/6];
        m.positions.push_back(pts[a]);
        m.normals.push_back(n);
        m.indices.push_back((uint32_t)m.indices.size());
        m.positions.push_back(pts[b]);
        m.normals.push_back(n);
        m.indices.push_back((uint32_t)m.indices.size());
        m.positions.push_back(pts[c]);
        m.normals.push_back(n);
        m.indices.push_back((uint32_t)m.indices.size());
    }
    return m;
}

MeshData Primitives::CreatePlane(float width, float depth)
{
    float hw=width*0.5f, hd=depth*0.5f;
    MeshData m;
    XMFLOAT3 p[4]={{-hw,0,-hd},{+hw,0,-hd},{+hw,0,+hd},{-hw,0,+hd}};
    XMFLOAT3 n={0,1,0};
    uint32_t idxs[]={0,1,2, 0,2,3};
    for(int i=0;i<6;i+=3){
        for(int j=0;j<3;j++){
            auto v=p[idxs[i+j]];
            m.positions.push_back(v);
            m.normals.push_back(n);
            m.indices.push_back((uint32_t)m.indices.size());
        }
    }
    return m;
}

MeshData Primitives::CreateSphere(float radius,int slices,int stacks)
{
    MeshData m;
    for(int i=0;i<=stacks;i++){
        float phi=(float)i/stacks*(float)M_PI;
        for(int j=0;j<=slices;j++){
            float theta=(float)j/slices*2*(float)M_PI;
            XMFLOAT3 pos={
                radius*sinf(phi)*cosf(theta),
                radius*cosf(phi),
                radius*sinf(phi)*sinf(theta)
            };
            XMFLOAT3 norm;
            XMVECTOR v=XMVector3Normalize(XMLoadFloat3(&pos));
            XMStoreFloat3(&norm,v);
            m.positions.push_back(pos);
            m.normals.push_back(norm);
        }
    }
    for(int i=0;i<stacks;i++){
        for(int j=0;j<slices;j++){
            int a=i*(slices+1)+j;
            int b=a+slices+1;
            m.indices.insert(m.indices.end(),{a,b,a+1, b,b+1,a+1});
        }
    }
    return m;
}