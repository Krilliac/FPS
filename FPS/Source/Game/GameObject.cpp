#include "GameObject.h"
#include "..\Utils\MathUtils.h"

UINT GameObject::s_nextID = 1;

GameObject::GameObject()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_worldMatrix(XMMatrixIdentity())
    , m_worldMatrixDirty(true)
    , m_device(nullptr)
    , m_context(nullptr)
    , m_isActive(true)
    , m_isVisible(true)
    , m_id(s_nextID++)
    , m_name("GameObject_" + std::to_string(m_id))
{
}

GameObject::~GameObject()
{
    Shutdown();
}

HRESULT GameObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    
    m_mesh = std::make_unique<Mesh>();
    HRESULT hr = m_mesh->Initialize(device, context);
    if (FAILED(hr)) return hr;
    
    CreateMesh();
    
    return S_OK;
}

void GameObject::Update(float deltaTime)
{
    // Base implementation - derived classes can override
    // Update world matrix if transforms changed
    if (m_worldMatrixDirty)
    {
        UpdateWorldMatrix();
    }
}

void GameObject::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    if (!m_isVisible || !m_mesh) return;
    
    // Ensure world matrix is up to date
    if (m_worldMatrixDirty)
    {
        UpdateWorldMatrix();
    }
    
    // Render the mesh
    m_mesh->Render();
}

void GameObject::Shutdown()
{
    m_mesh.reset();
    m_device = nullptr;
    m_context = nullptr;
}

void GameObject::SetPosition(const XMFLOAT3& position)
{
    m_position = position;
    m_worldMatrixDirty = true;
}

void GameObject::SetRotation(const XMFLOAT3& rotation)
{
    m_rotation = rotation;
    m_worldMatrixDirty = true;
}

void GameObject::SetScale(const XMFLOAT3& scale)
{
    m_scale = scale;
    m_worldMatrixDirty = true;
}

void GameObject::Translate(const XMFLOAT3& translation)
{
    m_position.x += translation.x;
    m_position.y += translation.y;
    m_position.z += translation.z;
    m_worldMatrixDirty = true;
}

void GameObject::Rotate(const XMFLOAT3& rotation)
{
    m_rotation.x += rotation.x;
    m_rotation.y += rotation.y;
    m_rotation.z += rotation.z;
    m_worldMatrixDirty = true;
}

void GameObject::Scale(const XMFLOAT3& scale)
{
    m_scale.x *= scale.x;
    m_scale.y *= scale.y;
    m_scale.z *= scale.z;
    m_worldMatrixDirty = true;
}

XMMATRIX GameObject::GetWorldMatrix()
{
    if (m_worldMatrixDirty)
    {
        UpdateWorldMatrix();
    }
    return m_worldMatrix;
}

XMFLOAT3 GameObject::GetForward() const
{
    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    forward = XMVector3TransformCoord(forward, rotMatrix);
    
    XMFLOAT3 result;
    XMStoreFloat3(&result, forward);
    return result;
}

XMFLOAT3 GameObject::GetRight() const
{
    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    right = XMVector3TransformCoord(right, rotMatrix);
    
    XMFLOAT3 result;
    XMStoreFloat3(&result, right);
    return result;
}

XMFLOAT3 GameObject::GetUp() const
{
    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    up = XMVector3TransformCoord(up, rotMatrix);
    
    XMFLOAT3 result;
    XMStoreFloat3(&result, up);
    return result;
}

float GameObject::GetDistanceFrom(const GameObject& other) const
{
    return GetDistanceFrom(other.GetPosition());
}

float GameObject::GetDistanceFrom(const XMFLOAT3& position) const
{
    float dx = m_position.x - position.x;
    float dy = m_position.y - position.y;
    float dz = m_position.z - position.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void GameObject::CreateMesh()
{
    // Base implementation creates a simple cube
    if (m_mesh)
    {
        m_mesh->CreateCube(1.0f);
    }
}

void GameObject::UpdateWorldMatrix()
{
    XMMATRIX scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMMATRIX translationMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
    
    m_worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
    m_worldMatrixDirty = false;
}

// CubeObject implementation
CubeObject::CubeObject(float size)
    : GameObject()
    , m_size(size)
{
    SetName("CubeObject_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void CubeObject::CreateMesh()
{
    if (m_mesh)
    {
        m_mesh->CreateCube(m_size);
    }
}

// PlaneObject implementation
PlaneObject::PlaneObject(float width, float depth)
    : GameObject()
    , m_width(width)
    , m_depth(depth)
{
    SetName("PlaneObject_" + std::to_string(GetID()));
}

HRESULT PlaneObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void PlaneObject::CreateMesh()
{
    if (m_mesh)
    {
        m_mesh->CreatePlane(m_width, m_depth);
    }
}

// SphereObject implementation
SphereObject::SphereObject(float radius, int slices, int stacks)
    : GameObject()
    , m_radius(radius)
    , m_slices(slices)
    , m_stacks(stacks)
{
    SetName("SphereObject_" + std::to_string(GetID()));
}

HRESULT SphereObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void SphereObject::CreateMesh()
{
    if (m_mesh)
    {
        m_mesh->CreateSphere(m_radius, m_slices, m_stacks);
    }
}
