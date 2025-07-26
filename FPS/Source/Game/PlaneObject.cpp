#include "PlaneObject.h"
#include "Primitives.h"

PlaneObject::PlaneObject(float width, float depth)
    : m_width(width)
    , m_depth(depth)
{
    SetName("PlaneObject_" + std::to_string(GetID()));
}

HRESULT PlaneObject::Initialize(ID3D11Device* dev, ID3D11DeviceContext* ctx)
{
    return GameObject::Initialize(dev, ctx);
}

void PlaneObject::CreateMesh()
{
    auto md = Primitives::CreatePlane(m_width, m_depth);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}