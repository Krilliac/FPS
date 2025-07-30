// SphereObject.cpp
#include "SphereObject.h"
#include "Utils/Assert.h"

SphereObject::SphereObject(float radius, int slices, int stacks)
    : m_radius(radius)
    , m_slices(slices)
    , m_stacks(stacks)
{
    ASSERT_MSG(radius > 0.0f, "Sphere radius must be positive");
    ASSERT_MSG(slices >= 3, "Sphere slices must be >= 3");
    ASSERT_MSG(stacks >= 2, "Sphere stacks must be >= 2");
    SetName("Sphere_" + std::to_string(GetID()));
}

HRESULT SphereObject::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
    ASSERT(d != nullptr);
    ASSERT(c != nullptr);
    return GameObject::Initialize(d, c);
}

void SphereObject::CreateMesh()
{
    auto md = Primitives::CreateSphere(m_radius, m_slices, m_stacks);
    ASSERT_ALWAYS_MSG(!md.vertices.empty() && !md.indices.empty(),
        "CreateSphere produced empty mesh");
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath);
    ASSERT(m_mesh);
}