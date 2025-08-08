#pragma once

#include "Game/GameObject.h"
#include "Game/PlaceholderMesh.h"
#include "Utils/Assert.h"
#include <DirectXMath.h>
#include <string>

class PyramidObject : public GameObject
{
public:
    PyramidObject(float size = 1.0f);
    ~PyramidObject() override = default;

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;
    void    Update(float dt) override { GameObject::Update(dt); }
    void    Render(const DirectX::XMMATRIX& v, const DirectX::XMMATRIX& p) override
    {
        GameObject::Render(v, p);
    }
    void    OnHit(GameObject*) override {}
    void    OnHitWorld(const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&) override {}

protected:
    void CreateMesh() override;

private:
    float        m_size;
    std::wstring m_modelPath{ L"Assets\\Models\\Pyramid.obj" };
};
