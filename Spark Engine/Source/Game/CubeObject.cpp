#include <Windows.h>
#include <cstdint>
#include <DirectXMath.h>
#include "CubeObject.h"

CubeObject::CubeObject(float size)
    : m_size(size)
{
    SetName("Cube_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
    return GameObject::Initialize(d, c);
}

void CubeObject::CreateMesh()
{
    auto md = Primitives::CreateCube(m_size);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}
