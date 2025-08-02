#pragma once
#include "../Core/framework.h"

#ifdef ENTT_AVAILABLE
#include <entt/entt.hpp>
#endif

namespace SparkEngine {
    using Entity = std::uint32_t;

    class EntityRegistry {
    public:
        EntityRegistry();
        ~EntityRegistry();

        Entity CreateEntity();
        void DestroyEntity(Entity e);
        bool IsValid(Entity e) const;

        template<typename C, typename... Args>
        C& AddComponent(Entity e, Args&&... args);

        template<typename C>
        C& GetComponent(Entity e);

        template<typename C>
        bool HasComponent(Entity e) const;

        template<typename C>
        void RemoveComponent(Entity e);

    private:
#ifdef ENTT_AVAILABLE
        entt::registry m_registry;
#else
        Entity m_next = 1;
#endif
    };
}
