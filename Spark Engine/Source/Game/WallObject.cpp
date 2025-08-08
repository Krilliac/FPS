﻿#include "WallObject.h"
#include <string>
#include <iostream>

WallObject::WallObject(float width, float height)
    : m_width(width), m_height(height)
{
    std::wcout << L"[INFO] WallObject constructed. width=" << width << L" height=" << height << std::endl;
    ASSERT_MSG(width > 0.f && height > 0.f, "Wall dimensions must be positive");
    SetName("Wall_" + std::to_string(GetID()));
}

HRESULT WallObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    std::wcout << L"[OPERATION] WallObject::Initialize called. width=" << m_width << L" height=" << m_height << std::endl;
    ASSERT(device != nullptr && context != nullptr);
    return GameObject::Initialize(device, context);
}

void WallObject::CreateMesh()
{
    std::wcout << L"[OPERATION] WallObject::CreateMesh called. width=" << m_width << L" height=" << m_height << std::endl;
    if (!m_mesh) {
        m_mesh = std::make_unique<Mesh>();
        std::wcout << L"[INFO] Mesh created for WallObject." << std::endl;
    }
    HRESULT hr = m_mesh->Initialize(m_device, m_context);
    std::wcout << L"[INFO] Mesh initialized for WallObject. HR=0x" << std::hex << hr << std::dec << std::endl;
    ASSERT_MSG(SUCCEEDED(hr), "Mesh initialization failed");
    bool loaded = false;
    if (!m_modelPath.empty()) {
        loaded = m_mesh->LoadFromFile(std::wstring(m_modelPath.begin(), m_modelPath.end()));
        std::wcout << L"[INFO] Mesh loaded from file for WallObject. loaded=" << loaded << std::endl;
    }
    if (!loaded) {
        hr = m_mesh->CreatePlane(m_width, m_height);
        std::wcout << L"[INFO] Procedural wall mesh created. HR=0x" << std::hex << hr << std::dec << std::endl;
        ASSERT_MSG(SUCCEEDED(hr), "Failed to create procedural wall mesh");
    }
    ASSERT_MSG(m_mesh && m_mesh->GetVertexCount() > 0 && m_mesh->GetIndexCount() > 0,
        "Wall mesh must have vertices and indices after loading");
}