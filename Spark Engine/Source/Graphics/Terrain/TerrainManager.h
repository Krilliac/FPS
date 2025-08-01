#pragma once
#include "../GraphicsEngine.h"
#include "../../ECS/EntityRegistry.h"
#include <DirectXMath.h>

namespace SparkEngine {
    struct TerrainChunk {
        DirectX::XMFLOAT2 worldPosition;
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
        uint32_t vertexCount;
        uint32_t indexCount;
        bool requiresUpdate = false;
        std::vector<float> heightData;
        std::vector<DirectX::XMFLOAT4> materialWeights; // 4 material layers
    };

    struct TerrainMaterial {
        std::string diffuseTexture;
        std::string normalTexture;
        float tileScale = 1.0f;
        DirectX::XMFLOAT4 materialColor = {1,1,1,1};
    };

    class TerrainManager {
    private:
        GraphicsEngine* m_graphics;
        EntityRegistry* m_registry;
        
        // Terrain configuration
        uint32_t m_chunkSize = 65; // 65x65 vertices per chunk
        float m_vertexSpacing = 2.0f;
        float m_maxElevation = 100.0f;
        
        // Chunk management
        std::unordered_map<uint64_t, std::unique_ptr<TerrainChunk>> m_chunks;
        std::vector<TerrainMaterial> m_terrainMaterials;
        
        // Rendering resources
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;
        ComPtr<ID3D11Buffer> m_constantBuffer;
        ComPtr<ID3D11SamplerState> m_samplerState;
        
        // Streaming
        DirectX::XMFLOAT3 m_observerPosition;
        float m_streamingRadius = 500.0f;

    public:
        TerrainManager(GraphicsEngine* graphics, EntityRegistry* registry);
        ~TerrainManager();

        bool Initialize();
        void Shutdown();
        void Update(float deltaTime);
        void Render(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projMatrix);

        // Terrain modification
        void ModifyElevation(const DirectX::XMFLOAT3& worldPos, float radius, float strength, bool raise = true);
        void SmoothTerrain(const DirectX::XMFLOAT3& worldPos, float radius, float strength);
        void PaintMaterial(const DirectX::XMFLOAT3& worldPos, float radius, int materialIndex, float strength);
        
        // Chunk management
        TerrainChunk* GetChunk(const DirectX::XMFLOAT2& chunkCoordinate);
        TerrainChunk* CreateChunk(const DirectX::XMFLOAT2& chunkCoordinate);
        void UpdateChunk(TerrainChunk* chunk);
        void UnloadDistantChunks();
        
        // Height queries
        float GetElevationAtPosition(const DirectX::XMFLOAT3& worldPos);
        DirectX::XMFLOAT3 GetNormalAtPosition(const DirectX::XMFLOAT3& worldPos);
        
        // Material management
        void AddTerrainMaterial(const TerrainMaterial& material);
        void RemoveTerrainMaterial(size_t index);
        void SetMaterialProperties(size_t index, const TerrainMaterial& material);
        
        // Streaming control
        void SetObserverPosition(const DirectX::XMFLOAT3& position) { m_observerPosition = position; }
        void SetStreamingRadius(float radius) { m_streamingRadius = radius; }

    private:
        uint64_t GetChunkKey(const DirectX::XMFLOAT2& chunkCoordinate);
        DirectX::XMFLOAT2 WorldToChunkCoordinate(const DirectX::XMFLOAT3& worldPos);
        void GenerateChunkGeometry(TerrainChunk* chunk);
        void CreateRenderingResources();
        void LoadTerrainTextures();
    };
}
