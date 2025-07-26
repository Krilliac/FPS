#include "CubeObject.h"

CubeObject::CubeObject(float size)
    : m_size(size)
{
    SetName("Cube_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void CubeObject::CreateMesh()
{
    auto md = Primitives::CreateCube(m_size);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}