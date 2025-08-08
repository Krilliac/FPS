#include "RampObject.h"
#include <string>

RampObject::RampObject(float length, float height)
    : m_length(length), m_height(height)
{
    ASSERT_MSG(length > 0.f && height > 0.f, "Ramp dimensions must be positive");
    SetName("Ramp_" + std::to_string(GetID()));
}

HRESULT RampObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT(device != nullptr && context != nullptr);
    return GameObject::Initialize(device, context);
}

void RampObject::CreateMesh()
{
    // FIXED: Create the mesh BEFORE calling LoadOrPlaceholderMesh
    if (!m_mesh) {
        m_mesh = std::make_unique<Mesh>();
    }

    // Now it's safe to call LoadOrPlaceholderMesh
    LoadOrPlaceholderMesh(*m_mesh, m_device, m_context, m_modelPath);

    // Ensure mesh is valid after loading
    ASSERT_MSG(m_mesh && m_mesh->GetVertexCount() > 0 && m_mesh->GetIndexCount() > 0,
        "Ramp mesh must have vertices and indices after loading");
}
