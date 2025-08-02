#pragma once
#include "../Core/framework.h"
#include "EntityRegistry.h"
#include "System.h"
#include "../Development/CollaborativeDevelopment.h"
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace SparkEngine {
    class SystemManager {
    public:
        SystemManager();
        ~SystemManager();

        bool Initialize(EntityRegistry* registry);
        void Shutdown();
        void ShutdownSystems();

        // System management
        template<typename T, typename... Args>
        T* AddSystem(Args&&... args) {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
            
            std::type_index typeIndex(typeid(T));
            
            // Check if system already exists
            if (m_systems.find(typeIndex) != m_systems.end()) {
                return static_cast<T*>(m_systems[typeIndex].get());
            }

            try {
                auto system = std::make_unique<T>(std::forward<Args>(args)...);
                T* systemPtr = system.get();
                
                if (!system->Initialize(m_registry)) {
                    std::cerr << "Failed to initialize system: " << typeid(T).name() << std::endl;
                    return nullptr;
                }

                m_systems[typeIndex] = std::move(system);
                m_systemOrder.push_back(systemPtr);
                
                std::cout << "âœ“ System added: " << typeid(T).name() << std::endl;
                return systemPtr;
            }
            catch (const std::exception& e) {
                std::cerr << "Exception adding system " << typeid(T).name() << ": " << e.what() << std::endl;
                return nullptr;
            }
        }

        template<typename T>
        void RemoveSystem() {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
            
            std::type_index typeIndex(typeid(T));
            auto it = m_systems.find(typeIndex);
            
            if (it != m_systems.end()) {
                // Remove from order vector
                auto orderIt = std::find(m_systemOrder.begin(), m_systemOrder.end(), it->second.get());
                if (orderIt != m_systemOrder.end()) {
                    m_systemOrder.erase(orderIt);
                }
                
                // Shutdown and remove
                it->second->Shutdown();
                m_systems.erase(it);
                
                std::cout << "System removed: " << typeid(T).name() << std::endl;
            }
        }

        template<typename T>
        T* GetSystem() {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
            
            std::type_index typeIndex(typeid(T));
            auto it = m_systems.find(typeIndex);
            
            if (it != m_systems.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        // System updates
        void UpdateSystems(float deltaTime);
        void RenderSystems();

        // System information
        size_t GetSystemCount() const { return m_systems.size(); }
        bool HasSystem(std::type_index typeIndex) const;

        // Collaborative development integration
        void SetCollaborativeDevelopment(CollaborativeDevelopment* collab) { 
            m_collaborativeDevelopment = collab; 
        }

    private:
        EntityRegistry* m_registry = nullptr;
        std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
        std::vector<System*> m_systemOrder;
        CollaborativeDevelopment* m_collaborativeDevelopment = nullptr;
        bool m_initialized = false;
    };

    // Implementation
    inline SystemManager::SystemManager() {
        std::cout << "SystemManager created" << std::endl;
    }

    inline SystemManager::~SystemManager() {
        Shutdown();
        std::cout << "SystemManager destroyed" << std::endl;
    }

    inline bool SystemManager::Initialize(EntityRegistry* registry) {
        if (!registry) {
            std::cerr << "Cannot initialize SystemManager with null registry" << std::endl;
            return false;
        }

        m_registry = registry;
        m_initialized = true;

        // Initialize collaborative development if enabled
        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Initialize("SystemManager", false);
        }

        std::cout << "SystemManager initialized" << std::endl;
        return true;
    }

    inline void SystemManager::Shutdown() {
        if (!m_initialized) {
            return;
        }

        ShutdownSystems();
        m_registry = nullptr;
        m_initialized = false;
        
        std::cout << "SystemManager shutdown" << std::endl;
    }

    inline void SystemManager::ShutdownSystems() {
        // Shutdown in reverse order
        for (auto it = m_systemOrder.rbegin(); it != m_systemOrder.rend(); ++it) {
            try {
                (*it)->Shutdown();
            }
            catch (const std::exception& e) {
                std::cerr << "Exception shutting down system: " << e.what() << std::endl;
            }
        }
        
        m_systems.clear();
        m_systemOrder.clear();
        
        std::cout << "All systems shutdown" << std::endl;
    }

    inline void SystemManager::UpdateSystems(float deltaTime) {
        if (!m_initialized || !m_registry) {
            return;
        }

        for (System* system : m_systemOrder) {
            if (system && system->IsEnabled()) {
                try {
                    system->Update(deltaTime);
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception in system update: " << e.what() << std::endl;
                    system->SetEnabled(false); // Disable problematic system
                }
            }
        }
    }

    inline void SystemManager::RenderSystems() {
        if (!m_initialized || !m_registry) {
            return;
        }

        for (System* system : m_systemOrder) {
            if (system && system->IsEnabled()) {
                try {
                    system->Render();
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception in system render: " << e.what() << std::endl;
                    // Continue rendering other systems
                }
            }
        }
    }

    inline bool SystemManager::HasSystem(std::type_index typeIndex) const {
        return m_systems.find(typeIndex) != m_systems.end();
    }
}
