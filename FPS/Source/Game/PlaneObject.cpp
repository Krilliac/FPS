#include "PlaneObject.h"

PlaneObject::PlaneObject(float width, float depth)
    : m_width(width), m_depth(depth)
{
    SetName("Plane_" + std::to_string(GetID()));
}

HRESULT PlaneObject::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void PlaneObject::CreateMesh()
{
    auto md = Primitives::CreatePlane(m_width, m_depth);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}