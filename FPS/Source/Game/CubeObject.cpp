#include "CubeObject.h"
#include "Primitives.h"

CubeObject::CubeObject(float size)
    : m_size(size)
{
    SetName("CubeObject_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* dev, ID3D11DeviceContext* ctx)
{
    return GameObject::Initialize(dev, ctx);
}

void CubeObject::CreateMesh()
{
    auto md = Primitives::CreateCube(m_size);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}