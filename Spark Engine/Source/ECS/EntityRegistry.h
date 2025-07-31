#pragma once
#include "../Core/framework.h"
#include <entt/entt.hpp>
#include <functional>
#include <unordered_map>
#include <memory>
#include <typeindex>

namespace SparkEngine {
    using Entity = entt::entity;

    class EntityRegistry {
    private:
        entt::registry m_registry;
        std::unordered_map<std::string, std::function<void(Entity)>> m_componentFactories;
        uint32_t m_entityCounter = 0;

    public:
        EntityRegistry() = default;
        ~EntityRegistry() = default;

        // Entity management
        Entity CreateEntity() {
            Entity entity = m_registry.create();
            m_entityCounter++;
            return entity;
        }

        void DestroyEntity(Entity entity) {
            if (m_registry.valid(entity)) {
                m_registry.destroy(entity);
            }
        }

        bool IsValid(Entity entity) const {
            return m_registry.valid(entity);
        }

        // Component management
        template<typename Component, typename... Args>
        Component& AddComponent(Entity entity, Args&&... args) {
            return m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
        }

        template<typename Component>
        Component& GetComponent(Entity entity) {
            return m_registry.get<Component>(entity);
        }

        template<typename Component>
        bool HasComponent(Entity entity) const {
            return m_registry.all_of<Component>(entity);
        }

        template<typename Component>
        void RemoveComponent(Entity entity) {
            m_registry.remove<Component>(entity);
        }

        // Views for system iteration
        template<typename... Components>
        auto View() {
            return m_registry.view<Components...>();
        }

        template<typename... Components>
        auto View() const {
            return m_registry.view<Components...>();
        }

        // Registry access for advanced operations
        entt::registry& GetRegistry() { return m_registry; }
        const entt::registry& GetRegistry() const { return m_registry; }

        uint32_t GetEntityCount() const { return m_entityCounter; }
    };
}
