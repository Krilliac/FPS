#include "PyramidObject.h"
#include <string>

PyramidObject::PyramidObject(float size)
    : m_size(size)
{
    ASSERT_MSG(size > 0.f, "Pyramid size must be positive");
    SetName("Pyramid_" + std::to_string(GetID()));
}

HRESULT PyramidObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT(device != nullptr && context != nullptr);
    return GameObject::Initialize(device, context);
}

void PyramidObject::CreateMesh()
{
    // FIXED: Create the mesh BEFORE calling LoadOrPlaceholderMesh
    if (!m_mesh) {
        m_mesh = std::make_unique<Mesh>();
    }

    // Now it's safe to call LoadOrPlaceholderMesh
    LoadOrPlaceholderMesh(*m_mesh, m_device, m_context, m_modelPath);

    // Ensure mesh is valid after loading
    ASSERT_MSG(m_mesh && m_mesh->GetVertexCount() > 0 && m_mesh->GetIndexCount() > 0,
        "Plane mesh must have vertices and indices after loading");
}
