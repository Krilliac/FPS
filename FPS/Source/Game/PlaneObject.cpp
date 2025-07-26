#include "PlaneObject.h"

PlaneObject::PlaneObject(float width, float depth)
    : m_width(width)
    , m_depth(depth)
{
    // Assign a unique name based on ID
    SetName("PlaneObject_" + std::to_string(GetID()));
}

HRESULT PlaneObject::Initialize(ID3D11Device* device,
                                ID3D11DeviceContext* context)
{
    // Use GameObject’s initialization for mesh setup
    return GameObject::Initialize(device, context);
}

void PlaneObject::CreateMesh()
{
    // Instruct the Mesh to generate a plane of the given dimensions
    if (m_mesh)
        m_mesh->CreatePlane(m_width, m_depth);
}