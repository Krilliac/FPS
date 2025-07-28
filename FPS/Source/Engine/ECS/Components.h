#pragma once
#include <entt/entt.hpp>
#include <DirectXMath.h>
#include <string>

using EntityID = entt::entity;

class World {
public:
    EntityID CreateEntity(const std::string& name = "");
    void DestroyEntity(EntityID entity);
    
    template<typename T, typename... Args>
    T& AddComponent(EntityID entity, Args&&... args) {
        return m_registry.emplace<T>(entity, std::forward<Args>(args)...);
    }
    
    template<typename T>
    T* GetComponent(EntityID entity) {
        return m_registry.try_get<T>(entity);
    }
    
    template<typename T>
    void RemoveComponent(EntityID entity) {
        m_registry.remove<T>(entity);
    }
    
    template<typename... Components>
    auto GetEntitiesWith() {
        return m_registry.view<Components...>();
    }
    
    entt::registry& GetRegistry() { return m_registry; }

private:
    entt::registry m_registry;
};

struct NameComponent {
    std::string name;
};

struct Transform {
    DirectX::XMFLOAT3 position{0,0,0};
    DirectX::XMFLOAT3 rotation{0,0,0};
    DirectX::XMFLOAT3 scale{1,1,1};
    EntityID parent = entt::null;
    
    DirectX::XMMATRIX GetWorldMatrix() const;
};

struct MeshRenderer {
    std::string meshPath;
    std::string materialPath;
    bool castShadows = true;
    bool receiveShadows = true;
    bool visible = true;
};

struct Camera {
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    bool isMainCamera = false;
};

struct Script {
    std::string scriptPath;
    std::string className;
    std::string moduleName;
    bool enabled = true;
    bool started = false;
};
