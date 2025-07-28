#include "SphereObject.h"

SphereObject::SphereObject(float radius, int slices, int stacks)
    : m_radius(radius), m_slices(slices), m_stacks(stacks)
{
    SetName("Sphere_" + std::to_string(GetID()));
}

HRESULT SphereObject::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
    return GameObject::Initialize(d, c);
}

void SphereObject::CreateMesh()
{
    auto md = Primitives::CreateSphere(m_radius, m_slices, m_stacks);
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);
}
