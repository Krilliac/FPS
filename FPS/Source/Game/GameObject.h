#pragma once

#include "..\Core\framework.h"  
#include "..\Graphics\Mesh.h"  
#include <memory>  

class GameObject  
{  
protected:  
    // Transform  
    XMFLOAT3 m_position;  
    XMFLOAT3 m_rotation;  
    XMFLOAT3 m_scale;  
    XMMATRIX m_worldMatrix;  
    bool m_worldMatrixDirty;  

    // Rendering  
    std::unique_ptr<Mesh> m_mesh;  
    ID3D11Device* m_device;  
    ID3D11DeviceContext* m_context;  

    // Update the base class `GameObject` to ensure the `Render` method is declared as virtual.  
    virtual void Render(const XMMATRIX& view, const XMMATRIX& projection) = 0;
    // Ensure the `Render` method in `Projectile` matches the base class signature.  
    //void Render(const XMMATRIX& view, const XMMATRIX& projection) override;
    // Ensure the `Render` method implementation in `Projectile` matches the base class signature.  
    void Projectile::Render(const XMMATRIX& view, const XMMATRIX& projection)  
    {  
        if (!m_active) return;  
        GameObject::Render(view, projection);  
    }
    // State  
    bool m_isActive;  
    bool m_isVisible;  

    // Identification  
    static UINT s_nextID;  
    UINT m_id;  
    std::string m_name;  

public:  
    GameObject();  
    virtual ~GameObject();  

    // Core functionality  
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);  
    virtual void Update(float deltaTime);  
    virtual void Render(const XMMATRIX& view, const XMMATRIX& projection);  
    virtual void Shutdown();  

    // Add the pure virtual function declaration inside the class  
    virtual void OnHit(GameObject* target) = 0;  
    virtual void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) = 0;  

    // Transform methods  
    void SetPosition(const XMFLOAT3& position);  
    void SetRotation(const XMFLOAT3& rotation);  
    void SetScale(const XMFLOAT3& scale);  

    void Translate(const XMFLOAT3& translation);  
    void Rotate(const XMFLOAT3& rotation);  
    void Scale(const XMFLOAT3& scale);  

    // Accessors  
    const XMFLOAT3& GetPosition() const { return m_position; }  
    const XMFLOAT3& GetRotation() const { return m_rotation; }  
    const XMFLOAT3& GetScale() const { return m_scale; }  

    XMMATRIX GetWorldMatrix();  
    XMFLOAT3 GetForward() const;  
    XMFLOAT3 GetRight() const;  
    XMFLOAT3 GetUp() const;  

    // State  
    bool IsActive() const { return m_isActive; }  
    bool IsVisible() const { return m_isVisible; }  
    void SetActive(bool active) { m_isActive = active; }  
    void SetVisible(bool visible) { m_isVisible = visible; }  

    // Identification  
    UINT GetID() const { return m_id; }  
    const std::string& GetName() const { return m_name; }  
    void SetName(const std::string& name) { m_name = name; }  

    // Mesh access  
    Mesh* GetMesh() const { return m_mesh.get(); }  

    // Distance calculation  
    float GetDistanceFrom(const GameObject& other) const;  
    float GetDistanceFrom(const XMFLOAT3& position) const;  

protected:  
    virtual void CreateMesh();  
    void UpdateWorldMatrix();  
};  

// Derived GameObject types  
class CubeObject : public GameObject  
{  
public:  
    CubeObject(float size = 1.0f);  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  

protected:  
    void CreateMesh() override;  

private:  
    float m_size;  
};  

class PlaneObject : public GameObject  
{  
public:  
    PlaneObject(float width = 10.0f, float depth = 10.0f);  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  

protected:  
    void CreateMesh() override;  

private:  
    float m_width;  
    float m_depth;  
};  

class SphereObject : public GameObject  
{  
public:  
    SphereObject(float radius = 1.0f, int slices = 20, int stacks = 20);  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  

protected:  
    void CreateMesh() override;  

private:  
    float m_radius;  
    int m_slices;  
    int m_stacks;  
};
