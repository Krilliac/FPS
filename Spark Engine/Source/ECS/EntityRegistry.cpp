#include "EntityRegistry.h"

namespace SparkEngine {
    EntityRegistry::EntityRegistry() = default;
    EntityRegistry::~EntityRegistry() = default;

    Entity EntityRegistry::CreateEntity() {
#ifdef ENTT_AVAILABLE
        return m_registry.create();
#else
        return m_next++;
#endif
    }

    void EntityRegistry::DestroyEntity(Entity e) {
#ifdef ENTT_AVAILABLE
        m_registry.destroy(e);
#endif
    }

    bool EntityRegistry::IsValid(Entity e) const {
#ifdef ENTT_AVAILABLE
        return m_registry.valid(e);
#else
        return e != 0;
#endif
    }

    template<typename C, typename... Args>
    C& EntityRegistry::AddComponent(Entity e, Args&&... args) {
#ifdef ENTT_AVAILABLE
        return m_registry.emplace<C>(e, std::forward<Args>(args)...);
#else
        static C comp;
        return comp;
#endif
    }

    template<typename C>
    C& EntityRegistry::GetComponent(Entity e) {
#ifdef ENTT_AVAILABLE
        return m_registry.get<C>(e);
#else
        static C comp;
        return comp;
#endif
    }

    template<typename C>
    bool EntityRegistry::HasComponent(Entity e) const {
#ifdef ENTT_AVAILABLE
        return m_registry.all_of<C>(e);
#else
        return false;
#endif
    }

    template<typename C>
    void EntityRegistry::RemoveComponent(Entity e) {
#ifdef ENTT_AVAILABLE
        m_registry.remove<C>(e);
#endif
    }

    // Explicit template instantiation for common components
    template TransformComponent& EntityRegistry::AddComponent<TransformComponent>(Entity, float, float, float);
    template CameraComponent& EntityRegistry::AddComponent<CameraComponent>(Entity);
    template RenderComponent& EntityRegistry::AddComponent<RenderComponent>(Entity);
    template LightComponent& EntityRegistry::AddComponent<LightComponent>(Entity);
    template PhysicsComponent& EntityRegistry::AddComponent<PhysicsComponent>(Entity);
}
