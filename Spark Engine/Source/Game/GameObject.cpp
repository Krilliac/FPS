#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <DirectXMath.h>
#include "GameObject.h"
#include "..\Utils\MathUtils.h"
#include "Utils/Assert.h"
#include <iostream>

using namespace DirectX;

// Definition and initialization of the static member
UINT GameObject::s_nextID = 1;

GameObject::GameObject()
    : m_position{ 0,0,0 }
    , m_rotation{ 0,0,0 }
    , m_scale{ 1,1,1 }
    , m_worldMatrix(XMMatrixIdentity())
    , m_worldMatrixDirty(true)
    , m_active(true)
    , m_visible(true)
    , m_id(s_nextID++)
    , m_name("GameObject_" + std::to_string(m_id))
{
    std::wcout << L"[INFO] GameObject constructed. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
}

GameObject::~GameObject()
{
    std::wcout << L"[INFO] GameObject destructor called. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    Shutdown();
}

HRESULT GameObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    std::wcout << L"[OPERATION] GameObject::Initialize called. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    ASSERT_MSG(device != nullptr, "GameObject::Initialize - device is null");
    ASSERT_MSG(context != nullptr, "GameObject::Initialize - context is null");
    m_device = device;
    m_context = context;
    m_mesh = std::make_unique<Mesh>();
    ASSERT(m_mesh);
    HRESULT hr = m_mesh->Initialize(device, context);
    std::wcout << L"[INFO] Mesh initialized for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << L" HR=0x" << std::hex << hr << std::dec << std::endl;
    ASSERT_MSG(SUCCEEDED(hr), "Mesh::Initialize failed");
    if (FAILED(hr)) {
        std::wcerr << L"[ERROR] Mesh::Initialize failed for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
        return hr;
    }
    CreateMesh();
    std::wcout << L"[INFO] GameObject::Initialize complete. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    return S_OK;
}

void GameObject::Shutdown()
{
    std::wcout << L"[OPERATION] GameObject::Shutdown called. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    if (m_mesh) m_mesh->Shutdown();
    m_mesh.reset();
    m_device = nullptr;
    m_context = nullptr;
    std::wcout << L"[INFO] GameObject shutdown complete. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
}

void GameObject::Update(float dt)
{
    std::wcout << L"[OPERATION] GameObject::Update called. ID=" << m_id << L" Name=" << m_name.c_str() << L" dt=" << dt << std::endl;
    if (m_worldMatrixDirty) UpdateWorldMatrix();
}

void GameObject::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    std::wcout << L"[OPERATION] GameObject::Render called. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    if (!m_visible || !m_mesh)
    {
        std::wcerr << L"[WARNING] [GameObject] Render skipped: visible=" << m_visible << L" mesh=" << (m_mesh ? L"OK" : L"NULL") << std::endl;
        return;
    }
    if (m_worldMatrixDirty) UpdateWorldMatrix();
    ASSERT(m_mesh);
    ASSERT_MSG(m_device != nullptr, "GameObject::Render - device is null");
    ASSERT_MSG(m_context != nullptr, "GameObject::Render - context is null");
    ASSERT_MSG(m_mesh->GetVertexCount() > 0 && m_mesh->GetIndexCount() > 0, "Mesh has no vertices or indices to render");
    std::wcout << L"[INFO] [GameObject] Rendering object ID=" << m_id << L" name=" << m_name.c_str() << L" verts=" << m_mesh->GetVertexCount() << L" inds=" << m_mesh->GetIndexCount() << std::endl;
    m_mesh->Render(m_context);
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
    ASSERT_MSG(std::isfinite(pos.x) && std::isfinite(pos.y) && std::isfinite(pos.z), "Invalid position vector");
    m_position = pos;
    m_worldMatrixDirty = true;
}

void GameObject::SetRotation(const XMFLOAT3& rot)
{
    ASSERT_MSG(std::isfinite(rot.x) && std::isfinite(rot.y) && std::isfinite(rot.z), "Invalid rotation vector");
    m_rotation = rot;
    m_worldMatrixDirty = true;
}

void GameObject::SetScale(const XMFLOAT3& scl)
{
    ASSERT_MSG(scl.x > 0 && scl.y > 0 && scl.z > 0, "Scale must be positive");
    m_scale = scl;
    m_worldMatrixDirty = true;
}

void GameObject::Translate(const XMFLOAT3& d)
{
    ASSERT_MSG(std::isfinite(d.x) && std::isfinite(d.y) && std::isfinite(d.z), "Invalid translation delta");
    m_position.x += d.x;
    m_position.y += d.y;
    m_position.z += d.z;
    m_worldMatrixDirty = true;
}

void GameObject::Rotate(const XMFLOAT3& d)
{
    ASSERT_MSG(std::isfinite(d.x) && std::isfinite(d.y) && std::isfinite(d.z), "Invalid rotation delta");
    m_rotation.x += d.x;
    m_rotation.y += d.y;
    m_rotation.z += d.z;
    m_worldMatrixDirty = true;
}

void GameObject::Scale(const XMFLOAT3& d)
{
    ASSERT_MSG(d.x > 0 && d.y > 0 && d.z > 0, "Scale factors must be positive");
    m_scale.x *= d.x;
    m_scale.y *= d.y;
    m_scale.z *= d.z;
    m_worldMatrixDirty = true;
}

XMMATRIX GameObject::GetWorldMatrix()
{
    if (m_worldMatrixDirty)
        UpdateWorldMatrix();
    return m_worldMatrix;
}

XMFLOAT3 GameObject::GetForward() const
{
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR fwd = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rot);
    XMFLOAT3 out; XMStoreFloat3(&out, fwd);
    return out;
}

XMFLOAT3 GameObject::GetRight() const
{
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR rt = XMVector3TransformCoord(XMVectorSet(1, 0, 0, 0), rot);
    XMFLOAT3 out; XMStoreFloat3(&out, rt);
    return out;
}

XMFLOAT3 GameObject::GetUp() const
{
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0, 1, 0, 0), rot);
    XMFLOAT3 out; XMStoreFloat3(&out, up);
    return out;
}

float GameObject::GetDistanceFrom(const GameObject& other) const
{
    return GetDistanceFrom(other.GetPosition());
}

float GameObject::GetDistanceFrom(const XMFLOAT3& pos) const
{
    ASSERT_MSG(std::isfinite(pos.x) && std::isfinite(pos.y) && std::isfinite(pos.z), "Invalid distance calculation position");
    float dx = m_position.x - pos.x;
    float dy = m_position.y - pos.y;
    float dz = m_position.z - pos.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void GameObject::CreateMesh()
{
    std::wcout << L"[OPERATION] GameObject::CreateMesh called. ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    if (!m_mesh) {
        m_mesh = std::make_unique<Mesh>();
        std::wcout << L"[INFO] Mesh created for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << std::endl;
    }
    HRESULT hr = m_mesh->Initialize(m_device, m_context);
    std::wcout << L"[INFO] Mesh initialized for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << L" HR=0x" << std::hex << hr << std::dec << std::endl;
    ASSERT_MSG(SUCCEEDED(hr), "Mesh initialization failed");
    bool loaded = false;
    if (!m_modelPath.empty()) {
        loaded = m_mesh->LoadFromFile(std::wstring(m_modelPath.begin(), m_modelPath.end()));
        std::wcout << L"[INFO] Mesh loaded from file for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << L" loaded=" << loaded << std::endl;
    }
    if (!loaded) {
        hr = m_mesh->CreateCube(1.0f);
        if (FAILED(hr)) {
            hr = m_mesh->CreateTriangle(1.0f);
        }
        if (FAILED(hr)) {
            hr = m_mesh->CreatePlane(2.0f, 2.0f);
        }
        std::wcout << L"[INFO] Procedural mesh created for GameObject ID=" << m_id << L" Name=" << m_name.c_str() << L" HR=0x" << std::hex << hr << std::dec << std::endl;
        ASSERT_MSG(SUCCEEDED(hr), "Failed to create procedural mesh");
    }
    ASSERT_MSG(m_mesh && m_mesh->GetVertexCount() > 0 && m_mesh->GetIndexCount() > 0,
        "Mesh must have vertices and indices after loading/creation");
    m_worldMatrixDirty = true;
    m_name = "GameObject_" + std::to_string(m_id);
    std::wcout << L"[INFO] GameObject created with ID: " << m_id << L" and name: " << m_name.c_str() << std::endl;
    ASSERT_MSG(!m_name.empty(), "GameObject name unexpected empty");
    ASSERT_MSG(m_device != nullptr, "GameObject device is null");
    ASSERT_MSG(m_context != nullptr, "GameObject context is null");
}

void GameObject::UpdateWorldMatrix()
{
    XMMATRIX S = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    XMMATRIX R = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMMATRIX T = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
    m_worldMatrix = S * R * T;
    m_worldMatrixDirty = false;
}