#include "CubeObject.h"
#include "Utils/Assert.h"          // custom assert
#include <DirectXMath.h>
#include <string>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

CubeObject::CubeObject(float size)
    : m_size(size)
{
    ASSERT_MSG(size > 0.0f, "Cube size must be positive");
    SetName("Cube_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT(device != nullptr);
    ASSERT(context != nullptr);
    return GameObject::Initialize(device, context);
}

void CubeObject::CreateMesh()
{
    // Generate primitive cube mesh data
    auto md = Primitives::CreateCube(m_size);
    ASSERT_MSG(!md.vertices.empty() && !md.indices.empty(), "Primitive cube mesh is empty");

    // Load model from file or placeholder if missing
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath, md);

    // Ensure mesh was assigned
    ASSERT(m_mesh != nullptr);
}