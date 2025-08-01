#pragma once
#include "../Core/framework.h"
#include "../ECS/EntityRegistry.h"
#include "../Graphics/Terrain/TerrainManager.h"
#include "../Development/MultiUserFramework.h"

namespace SparkEngine {
    struct ModificationBrush {
        enum Type { Heightmap, Texture, Smooth, Flatten };
        Type operation = Heightmap;
        float radius = 10.0f;
        float intensity = 0.5f;
        float falloff = 0.8f;
        int layerIndex = 0;
    };

    class ContentSynchronizer {
    private:
        EntityRegistry* m_registry;
        MultiUserFramework* m_devFramework;
        class TerrainManager* m_terrainManager;
        
        // Synchronization state
        ModificationBrush m_activeBrush;
        bool m_isModifying = false;
        DirectX::XMFLOAT3 m_lastModificationPoint;
        
        // Streaming system
        std::unordered_map<std::string, Entity> m_streamedRegions;
        float m_synchronizationRadius = 500.0f;

    public:
        ContentSynchronizer(EntityRegistry* registry, MultiUserFramework* framework);
        ~ContentSynchronizer();

        bool Initialize();
        void Update(float deltaTime);
        void Render();

        // Content modification
        void BeginContentModification(const DirectX::XMFLOAT3& position);
        void UpdateContentModification(const DirectX::XMFLOAT3& position);
        void EndContentModification();
        
        // Brush configuration
        void SetBrushOperation(ModificationBrush::Type type) { m_activeBrush.operation = type; }
        void SetBrushRadius(float radius) { m_activeBrush.radius = radius; }
        void SetBrushIntensity(float intensity) { m_activeBrush.intensity = intensity; }
        
        // Entity operations
        Entity CreateSynchronizedEntity(const std::string& prefabName, const DirectX::XMFLOAT3& position);
        bool ModifySynchronizedEntity(Entity entity, const DirectX::XMFLOAT3& newPosition);
        bool RemoveSynchronizedEntity(Entity entity);
        
        // Region management
        void UpdateRegionStreaming(const DirectX::XMFLOAT3& viewerPosition);
        void LoadContentRegion(const std::string& regionId);
        void UnloadContentRegion(const std::string& regionId);
        
        // Developer visualization
        void ShowDeveloperIndicators(bool show);
        void BroadcastModification(const DirectX::XMFLOAT3& position, const ModificationBrush& brush);

    private:
        void ApplyModificationBrush(const DirectX::XMFLOAT3& position, const ModificationBrush& brush);
        void HandleSynchronizedModification(const DevelopmentEvent& event);
        std::string GetRegionId(const DirectX::XMFLOAT3& position);
    };
}
