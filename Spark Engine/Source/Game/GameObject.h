#pragma once

#include "..\Core\framework.h"    // XMFLOAT3, XMMATRIX, HRESULT
#include "..\Graphics\Mesh.h"
#include <memory>
#include <string>

// Forward‐declare Projectile to avoid include cycles
namespace Projectiles { class Projectile; }
using Projectiles::Projectile;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    // Core lifecycle
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    virtual void    Update(float deltaTime);
    virtual void    Render(const XMMATRIX& view, const XMMATRIX& projection);
    virtual void    Shutdown();

    // Hit callbacks (must be overridden by subclasses)
    virtual void OnHit(GameObject* target) = 0;
    virtual void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) = 0;

    // Transform
    void SetPosition(const XMFLOAT3& pos);
    void SetRotation(const XMFLOAT3& rot);
    void SetScale(const XMFLOAT3& scale);
    void Translate(const XMFLOAT3& delta);
    void Rotate(const XMFLOAT3& delta);
    void Scale(const XMFLOAT3& delta);

    // Accessors
    const XMFLOAT3& GetPosition() const { return m_position; }
    const XMFLOAT3& GetRotation() const { return m_rotation; }
    const XMFLOAT3& GetScale()    const { return m_scale; }

    XMMATRIX GetWorldMatrix();
    XMFLOAT3 GetForward() const;
    XMFLOAT3 GetRight()   const;
    XMFLOAT3 GetUp()      const;

    bool IsActive()  const { return m_active; }
    bool IsVisible() const { return m_visible; }
    void SetActive(bool v) { m_active = v; }
    void SetVisible(bool v) { m_visible = v; }

    UINT GetID()                   const { return m_id; }
    const std::string& GetName()   const { return m_name; }
    void SetName(const std::string& n) { m_name = n; }

    Mesh* GetMesh() const { return m_mesh.get(); }

    float GetDistanceFrom(const GameObject& o) const;
    float GetDistanceFrom(const XMFLOAT3& p) const;

protected:
    // Internal helpers
    virtual void CreateMesh();
    void        UpdateWorldMatrix();

    // Transform state
    XMFLOAT3             m_position{};
    XMFLOAT3             m_rotation{};
    XMFLOAT3             m_scale{ 1,1,1 };
    XMMATRIX             m_worldMatrix{};
    bool                 m_worldMatrixDirty{ true };

    // Rendering
    std::unique_ptr<Mesh> m_mesh;
    ID3D11Device* m_device{ nullptr };
    ID3D11DeviceContext* m_context{ nullptr };

    // Visibility/activation
    bool m_active{ true };
    bool m_visible{ true };

    // Identification
    static UINT   s_nextID;   // declaration only
    UINT          m_id{ 0 };
    std::string   m_name;

private:
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
};
