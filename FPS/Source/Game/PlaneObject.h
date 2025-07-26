#pragma once

#include "GameObject.h"
#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class PlaneObject : public GameObject
{
public:
    // Constructor: specify plane width and depth
    PlaneObject(float width = 10.0f, float depth = 10.0f);
    ~PlaneObject() override = default;

    // Initialize base GameObject (mesh, etc.)
    HRESULT Initialize(ID3D11Device* device,
                       ID3D11DeviceContext* context) override;

    // Update and render reuse base implementations
    void Update(float deltaTime) override     { GameObject::Update(deltaTime); }
    void Render(const XMMATRIX& view,
                const XMMATRIX& projection) override
    {
        GameObject::Render(view, projection);
    }

    // Satisfy the pure-virtual interface so PlaneObject is concrete
    void OnHit(GameObject* target) override       { /* no-op */ }
    void OnHitWorld(const XMFLOAT3& hitPoint,
                    const XMFLOAT3& normal) override
    {
        /* no-op */
    }

protected:
    // Create the plane mesh
    void CreateMesh() override;

private:
    float m_width;
    float m_depth;
};