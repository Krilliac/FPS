#include "CubeObject.h"

CubeObject::CubeObject(float size)
    : m_size(size)
{
    // Assign a unique name based on ID
    SetName("CubeObject_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* device,
                               ID3D11DeviceContext* context)
{
    // Use GameObject’s initialization for mesh setup
    return GameObject::Initialize(device, context);
}

void CubeObject::CreateMesh()
{
    // Instruct the Mesh to generate a cube of the given size
    if (m_mesh)
        m_mesh->CreateCube(m_size);
}