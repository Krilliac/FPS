#pragma once
#include "EntityRegistry.h"
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace SparkEngine {
    class System {
    public:
        virtual ~System() = default;
        virtual void Initialize(EntityRegistry* registry) { m_registry = registry; }
        virtual void Update(float deltaTime) = 0;
        virtual void Shutdown() {}
        virtual const char* GetName() const = 0;

    protected:
        EntityRegistry* m_registry = nullptr;
    };

    class SystemManager {
    private:
        std::vector<std::unique_ptr<System>> m_systems;
        EntityRegistry* m_registry = nullptr;
        bool m_initialized = false;

    public:
        SystemManager() = default;
        ~SystemManager() = default;

        void Initialize(EntityRegistry* registry) {
            m_registry = registry;
            m_initialized = true;
        }

        template<typename T, typename... Args>
        T* AddSystem(Args&&... args) {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");

            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = system.get();

            if (m_initialized) {
                system->Initialize(m_registry);
            }

            m_systems.push_back(std::move(system));
            return ptr;
        }

        void UpdateSystems(float deltaTime) {
            for (auto& system : m_systems) {
                system->Update(deltaTime);
            }
        }

        void ShutdownSystems() {
            for (auto& system : m_systems) {
                system->Shutdown();
            }
            m_systems.clear();
        }

        size_t GetSystemCount() const { return m_systems.size(); }
    };
}
