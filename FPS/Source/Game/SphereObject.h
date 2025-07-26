#pragma once

#include "GameObject.h"
#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class SphereObject : public GameObject
{
public:
    // Constructor: radius, slices, stacks
    SphereObject(float radius = 1.0f, int slices = 20, int stacks = 20);
    ~SphereObject() override = default;

    // Initialize base GameObject (mesh, transforms)
    HRESULT Initialize(ID3D11Device* device,
                       ID3D11DeviceContext* context) override;

    // Use base Update/Render
    void Update(float deltaTime) override   { GameObject::Update(deltaTime); }
    void Render(const XMMATRIX& view,
                const XMMATRIX& projection) override
    {
        GameObject::Render(view, projection);
    }

    // Implement pure-virtual callbacks so SphereObject is concrete
    void OnHit(GameObject* target) override       { /* no-op */ }
    void OnHitWorld(const XMFLOAT3& hitPoint,
                    const XMFLOAT3& normal) override
    {
        /* no-op */
    }

protected:
    // Called by Initialize to build the mesh
    void CreateMesh() override;

private:
    float m_radius;
    int   m_slices;
    int   m_stacks;
};