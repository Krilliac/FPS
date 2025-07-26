#include "SphereObject.h"

SphereObject::SphereObject(float radius, int slices, int stacks)
    : m_radius(radius)
    , m_slices(slices)
    , m_stacks(stacks)
{
    // Give each instance a unique name for debugging
    SetName("SphereObject_" + std::to_string(GetID()));
}

HRESULT SphereObject::Initialize(ID3D11Device* device,
                                 ID3D11DeviceContext* context)
{
    // Base sets up mesh, world matrix, etc.
    return GameObject::Initialize(device, context);
}

void SphereObject::CreateMesh()
{
    // Build a UV sphere with given parameters
    if (m_mesh)
        m_mesh->CreateSphere(m_radius, m_slices, m_stacks);
}