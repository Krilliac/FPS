#pragma once
#include "../Core/framework.h"
#include "../Development/MultiUserFramework.h"
#include "../Content/ContentSynchronizer.h"
#include "../Networking/NetworkCore.h"
#include "../Assets/DynamicAssetPipeline.h"
#include "../Scripting/DistributedScriptManager.h"
#include "../Assets/StreamingAssetManager.h"
#include "../Graphics/Terrain/TerrainManager.h"

namespace SparkEngine {
    class CollaborativeDevelopment {
    private:
        // Core SparkEngine systems
        EntityRegistry* m_registry;
        AssetManager* m_assetManager;
        ScriptingSystem* m_scriptingSystem;
        GraphicsEngine* m_graphics;
        
        // Collaborative development systems
        std::unique_ptr<MultiUserFramework> m_multiUserFramework;
        std::unique_ptr<ContentSynchronizer> m_contentSynchronizer;
        std::unique_ptr<NetworkCore> m_networkCore;
        std::unique_ptr<DynamicAssetPipeline> m_assetPipeline;
        std::unique_ptr<DistributedScriptManager> m_distributedScripts;
        std::unique_ptr<StreamingAssetManager> m_streamingAssets;
        std::unique_ptr<TerrainManager> m_terrainManager;
        
        bool m_initialized = false;
        bool m_collaborativeMode = false;

    public:
        CollaborativeDevelopment(EntityRegistry* registry, AssetManager* assetManager, 
                               ScriptingSystem* scripting, GraphicsEngine* graphics);
        ~CollaborativeDevelopment();

        // Initialization
        bool Initialize(const std::string& developerId, bool enableCollaboration = false);
        void Shutdown();
        void Update(float deltaTime);

        // Development session features
        bool CreateDevelopmentSession(const std::string& sessionId);
        bool JoinDevelopmentSession(const std::string& sessionId, const std::string& hostAddress);
        void EndDevelopmentSession();
        
        // Content development
        void EnableRealTimeContentSync(bool enable);
        void SetContentBrushSettings(float radius, float intensity);
        void ModifyContent(const DirectX::XMFLOAT3& position, bool elevate = true);
        
        // Distributed networking
        bool StartNetworkHost(uint16_t port = 7777);
        bool ConnectToNetworkHost(const std::string& hostAddress, uint16_t port = 7777);
        void SendDeveloperAction(const std::string& action, const std::string& data);
        
        // Dynamic asset management
        void EnableAssetStreaming(bool enable);
        void RequestAsset(const std::string& assetId);
        void NotifyAssetModified(const std::string& assetId);
        
        // Distributed scripting
        void LoadDistributedScript(const std::string& scriptName, const std::string& source);
        void ExecuteDistributedEvent(const std::string& eventName, const std::string& data);
        
        // Streaming asset control
        void EnableStreamingAssets(bool enable);
        void SetStreamingDistance(float distance);
        void PreloadArea(const DirectX::XMFLOAT3& center, float radius);

        // System accessors
        MultiUserFramework* GetMultiUserFramework() const { return m_multiUserFramework.get(); }
        ContentSynchronizer* GetContentSynchronizer() const { return m_contentSynchronizer.get(); }
        NetworkCore* GetNetworkCore() const { return m_networkCore.get(); }
        TerrainManager* GetTerrainManager() const { return m_terrainManager.get(); }
        
        // Status queries
        bool IsCollaborative() const;
        bool IsNetworkingEnabled() const { return m_collaborativeMode; }
        size_t GetConnectedDeveloperCount() const;
        std::vector<std::string> GetConnectedDevelopers() const;

    private:
        void InitializeMultiUserDevelopment(const std::string& developerId);
        void InitializeNetworkingSystems();
        void SetupDevelopmentEventHandlers();
        void HandleDevelopmentEvent(const DevelopmentEvent& event);
    };
}
