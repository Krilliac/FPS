#include "SphereObject.h"

SphereObject::SphereObject(float radius,
    int slices,
    int stacks)
    : m_radius(radius)
    , m_slices(slices)
    , m_stacks(stacks)
{
    SetName("Sphere_" + std::to_string(GetID()));
}

HRESULT SphereObject::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void SphereObject::CreateMesh()
{
    auto md = Primitives::CreateSphere(m_radius, m_slices, m_stacks);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}