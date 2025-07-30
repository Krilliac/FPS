#include "PlaneObject.h"
#include "Utils/Assert.h"
#include <string>

PlaneObject::PlaneObject(float width, float depth)
    : m_width(width), m_depth(depth)
{
    ASSERT_MSG(width > 0.f && depth > 0.f, "Plane dimensions must be positive");
    SetName("Plane_" + std::to_string(GetID()));
}

HRESULT PlaneObject::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
    ASSERT(d != nullptr);
    ASSERT(c != nullptr);
    return GameObject::Initialize(d, c);
}

void PlaneObject::CreateMesh()
{
    auto md = Primitives::CreatePlane(m_width, m_depth);
    ASSERT_ALWAYS_MSG(!md.vertices.empty() && !md.indices.empty(),
        "Generated plane mesh is empty");

    LoadOrPlaceholderMesh(*m_mesh, m_modelPath);

    // Ensure mesh is valid
    ASSERT(m_mesh != nullptr);
}