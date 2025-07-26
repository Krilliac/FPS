#pragma once

#include "GameObject.h"
#include "PlaceholderMesh.h"
#include "Primitives.h"
#include <DirectXMath.h>
using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class PlaneObject : public GameObject
{
public:
    PlaneObject(float width = 10.0f, float depth = 10.0f);
    ~PlaneObject() override = default;

    HRESULT Initialize(ID3D11Device* device,
        ID3D11DeviceContext* context) override;

    void Update(float dt) override { GameObject::Update(dt); }
    void Render(const XMMATRIX& v,
        const XMMATRIX& p) override {
        GameObject::Render(v, p);
    }

    void OnHit(GameObject*) override {}
    void OnHitWorld(const XMFLOAT3&, const XMFLOAT3&) override {}

protected:
    void CreateMesh() override;

private:
    float         m_width, m_depth;
    std::wstring  m_modelPath = L"Assets/Models/Plane.fbx";
};