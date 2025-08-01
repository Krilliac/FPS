#pragma once
#include "../Core/framework.h"
#include "../Assets/AssetManager.h"
#include "../Networking/NetworkCore.h"
#include <unordered_set>

namespace SparkEngine {
    enum class AssetType {
        Mesh, Texture, Animation, Audio, Script, Material, Scene, Prefab
    };

    struct AssetRequest {
        std::string assetId;
        AssetType type;
        std::string requesterId;
        uint64_t timestamp;
        uint32_t priority = 1;
    };

    class DynamicAssetPipeline {
    private:
        AssetManager* m_assetManager;
        NetworkCore* m_networkCore;
        
        // Asset streaming
        std::unordered_set<std::string> m_streamedAssets;
        std::queue<AssetRequest> m_pendingRequests;
        std::mutex m_requestMutex;
        
        // Version tracking
        std::unordered_map<std::string, uint64_t> m_assetVersions;
        std::unordered_map<std::string, std::string> m_assetHashes;
        
        // Pipeline parameters
        float m_streamingRadius = 200.0f;
        size_t m_maxConcurrentStreams = 10;
        size_t m_cacheSize = 500 * 1024 * 1024; // 500MB

        std::thread m_pipelineThread;
        std::atomic<bool> m_isRunning{false};

    public:
        DynamicAssetPipeline(AssetManager* assetManager, NetworkCore* networkCore);
        ~DynamicAssetPipeline();

        bool Initialize();
        void Shutdown();
        void Update();

        // Asset streaming
        void RequestAsset(const std::string& assetId, AssetType type, uint32_t priority = 1);
        void StreamAssetsInRadius(const DirectX::XMFLOAT3& position);
        void UnstreamAsset(const std::string& assetId);
        
        // Hot reloading
        void NotifyAssetModified(const std::string& assetId);
        void UpdateAsset(const std::string& assetId, const std::vector<uint8_t>& newData);
        bool IsAssetCurrent(const std::string& assetId) const;
        
        // Asset validation
        bool ValidateAsset(const std::string& assetId, const std::string& expectedHash);
        std::string CalculateAssetHash(const std::vector<uint8_t>& data);
        
        // Runtime asset replacement
        void HotSwapAsset(const std::string& assetId, const std::vector<uint8_t>& newAssetData);
        void ReloadModifiedAssets();
        
        // Asset discovery
        std::vector<std::string> DiscoverAssetsInRange(const DirectX::XMFLOAT3& position, float radius);
        std::vector<std::string> GetAssetDependencies(const std::string& assetId);

        // Pipeline statistics
        size_t GetStreamedAssetCount() const { return m_streamedAssets.size(); }
        size_t GetPendingRequestCount() const { return m_pendingRequests.size(); }

    private:
        void PipelineThreadFunction();
        void ProcessAssetRequest(const AssetRequest& request);
        void HandleAssetResponse(const NetworkPacket& packet);
        bool ShouldStreamAsset(const std::string& assetId, const DirectX::XMFLOAT3& position);
    };
}
