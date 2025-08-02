#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class EntityRegistry;

    class System {
    public:
        System() = default;
        virtual ~System() = default;

        // Pure virtual functions that derived systems must implement
        virtual bool Initialize(EntityRegistry* registry) = 0;
        virtual void Shutdown() = 0;
        virtual void Update(float deltaTime) = 0;

        // Optional virtual functions
        virtual void Render() {}
        virtual void OnEntityCreated(Entity entity) {}
        virtual void OnEntityDestroyed(Entity entity) {}

        // System state
        bool IsEnabled() const { return m_enabled; }
        void SetEnabled(bool enabled) { m_enabled = enabled; }

        // Registry access
        EntityRegistry* GetRegistry() const { return m_registry; }

    protected:
        EntityRegistry* m_registry = nullptr;
        bool m_enabled = true;
    };
}
