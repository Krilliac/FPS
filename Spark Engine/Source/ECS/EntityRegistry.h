#pragma once
#include "../Core/framework.h"

#ifdef ENTT_AVAILABLE
#include <entt/entt.hpp>
#endif

namespace SparkEngine {
    class EntityRegistry {
    public:
        EntityRegistry()=default; ~EntityRegistry()=default;
        Entity CreateEntity() {
#ifdef ENTT_AVAILABLE
            return m_registry.create();
#else
            return m_next++;
#endif
        }
        void DestroyEntity(Entity e) {
#ifdef ENTT_AVAILABLE
            m_registry.destroy(e);
#endif
        }
        bool IsValid(Entity e) const {
#ifdef ENTT_AVAILABLE
            return m_registry.valid(e);
#else
            return e!=0;
#endif
        }
        template<typename C,typename... A>
        decltype(auto) AddComponent(Entity e,A&&... a){
#ifdef ENTT_AVAILABLE
            return m_registry.emplace<C>(e,std::forward<A>(a)...);
#else
            static C d; return d;
#endif
        }
        template<typename C>
        C& GetComponent(Entity e){
#ifdef ENTT_AVAILABLE
            return m_registry.get<C>(e);
#else
            static C d; return d;
#endif
        }
        template<typename C>
        bool HasComponent(Entity e) const {
#ifdef ENTT_AVAILABLE
            return m_registry.all_of<C>(e);
#else
            return false;
#endif
        }
        template<typename C>
        void RemoveComponent(Entity e){
#ifdef ENTT_AVAILABLE
            m_registry.remove<C>(e);
#endif
        }
    private:
#ifdef ENTT_AVAILABLE
        entt::registry m_registry;
#else
        Entity m_next{1};
#endif
    };
}
