#include "TerrainManager.h"
#include "../GraphicsEngine.h"
#include "../../Utils/Assert.h"
#include <algorithm>
#include <iostream>
#include <Game/Terrain.h>

namespace SparkEngine {
    TerrainManager::TerrainManager(GraphicsEngine* graphics, EntityRegistry* registry)
        : m_graphics(graphics), m_registry(registry) {
    }

    TerrainManager::~TerrainManager() {
        Shutdown();
    }

    bool TerrainManager::Initialize() {
        SPARK_ASSERT(m_graphics);

        if (!CreateRenderingResources()) {
            std::cerr << "Failed to create terrain rendering resources" << std::endl;
            return false;
        }

        LoadTerrainTextures();

        // Create some default terrain materials
        TerrainMaterial grass;
        grass.diffuseTexture = "grass_diffuse.png";
        grass.normalTexture = "grass_normal.png";
        grass.tileScale = 16.0f;
        AddTerrainMaterial(grass);

        TerrainMaterial rock;
        rock.diffuseTexture = "rock_diffuse.png";
        rock.normalTexture = "rock_normal.png";
        rock.tileScale = 8.0f;
        AddTerrainMaterial(rock);

        std::cout << "TerrainManager initialized successfully" << std::endl;
        return true;
    }

    void TerrainManager::Shutdown() {
        m_chunks.clear();
        m_terrainMaterials.clear();
        
        m_samplerState.Reset();
        m_constantBuffer.Reset();
        m_inputLayout.Reset();
        m_pixelShader.Reset();
        m_vertexShader.Reset();
    }

    void TerrainManager::Update(float deltaTime) {
        // Update streaming based on observer position
        UnloadDistantChunks();
        
        // Update chunks that need mesh regeneration
        for (auto& [key, chunk] : m_chunks) {
            if (chunk->requiresUpdate) {
                UpdateChunk(chunk.get());
                chunk->requiresUpdate = false;
            }
        }
    }

    void TerrainManager::Render(const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix) {
        if (!m_graphics || !m_graphics->GetContext()) return;

        auto context = m_graphics->GetContext();

        // Set shaders
        context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
        context->IASetInputLayout(m_inputLayout.Get());

        // Set constant buffer
        struct TerrainConstants {
            XMMATRIX world;
            XMMATRIX view;
            XMMATRIX projection;
            XMFLOAT3 observerPos;
            float vertexSpacing;
        } constants;

        constants.world = XMMatrixIdentity();
        constants.view = viewMatrix;
        constants.projection = projMatrix;
        constants.observerPos = m_observerPosition;
        constants.vertexSpacing = m_vertexSpacing;

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if (SUCCEEDED(context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
            memcpy(mappedResource.pData, &constants, sizeof(constants));
            context->Unmap(m_constantBuffer.Get(), 0);
        }

        context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
        context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        // Set sampler
        context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        // Render visible chunks
        for (const auto& [key, chunk] : m_chunks) {
            if (IsChunkVisible(chunk.get(), viewMatrix, projMatrix)) {
                RenderChunk(context, chunk.get());
            }
        }
    }

    void TerrainManager::ModifyElevation(const XMFLOAT3& worldPos, float radius, float strength, bool raise) {
        // Find affected chunks
        std::vector<TerrainChunk*> affectedChunks;
        
        XMFLOAT2 centerChunk = WorldToChunkCoordinate(worldPos);
        int chunkRadius = static_cast<int>(std::ceil(radius / (m_chunkSize * m_vertexSpacing))) + 1;
        
        for (int x = -chunkRadius; x <= chunkRadius; ++x) {
            for (int z = -chunkRadius; z <= chunkRadius; ++z) {
                XMFLOAT2 chunkCoord = {centerChunk.x + x, centerChunk.y + z};
                TerrainChunk* chunk = GetChunk(chunkCoord);
                if (!chunk) {
                    chunk = CreateChunk(chunkCoord);
                }
                if (chunk) {
                    affectedChunks.push_back(chunk);
                }
            }
        }

        // Modify height data
        for (TerrainChunk* chunk : affectedChunks) {
            bool chunkModified = false;
            
            for (uint32_t z = 0; z < m_chunkSize; ++z) {
                for (uint32_t x = 0; x < m_chunkSize; ++x) {
                    XMFLOAT3 vertexWorldPos = {
                        chunk->worldPosition.x + x * m_vertexSpacing,
                        0.0f,
                        chunk->worldPosition.y + z * m_vertexSpacing
                    };
                    
                    float distance = sqrt(
                        (vertexWorldPos.x - worldPos.x) * (vertexWorldPos.x - worldPos.x) +
                        (vertexWorldPos.z - worldPos.z) * (vertexWorldPos.z - worldPos.z)
                    );
                    
                    if (distance <= radius) {
                        float influence = 1.0f - (distance / radius);
                        influence = influence * influence; // Smooth falloff
                        
                        uint32_t index = z * m_chunkSize + x;
                        float heightChange = strength * influence * (raise ? 1.0f : -1.0f);
                        chunk->heightData[index] = std::max(0.0f, 
                            std::min(m_maxElevation, chunk->heightData[index] + heightChange));
                        
                        chunkModified = true;
                    }
                }
            }
            
            if (chunkModified) {
                chunk->requiresUpdate = true;
            }
        }
    }

    TerrainChunk* TerrainManager::GetChunk(const XMFLOAT2& chunkCoordinate) {
        uint64_t key = GetChunkKey(chunkCoordinate);
        auto it = m_chunks.find(key);
        return (it != m_chunks.end()) ? it->second.get() : nullptr;
    }

    TerrainChunk* TerrainManager::CreateChunk(const XMFLOAT2& chunkCoordinate) {
        uint64_t key = GetChunkKey(chunkCoordinate);
        
        auto chunk = std::make_unique<TerrainChunk>();
        chunk->worldPosition = {
            chunkCoordinate.x * (m_chunkSize - 1) * m_vertexSpacing,
            chunkCoordinate.y * (m_chunkSize - 1) * m_vertexSpacing
        };
        
        // Initialize height data (flat for now, could load from heightmap)
        chunk->heightData.resize(m_chunkSize * m_chunkSize, 0.0f);
        chunk->materialWeights.resize(m_chunkSize * m_chunkSize, {1.0f, 0.0f, 0.0f, 0.0f});
        
        GenerateChunkGeometry(chunk.get());
        
        TerrainChunk* chunkPtr = chunk.get();
        m_chunks[key] = std::move(chunk);
        
        return chunkPtr;
    }

    void TerrainManager::UpdateChunk(TerrainChunk* chunk) {
        if (!chunk) return;
        GenerateChunkGeometry(chunk);
    }

    void TerrainManager::GenerateChunkGeometry(TerrainChunk* chunk) {
        if (!chunk || !m_graphics) return;

        std::vector<TerrainVertex> vertices;
        std::vector<uint32_t> indices;

        // Generate vertices
        for (uint32_t z = 0; z < m_chunkSize; ++z) {
            for (uint32_t x = 0; x < m_chunkSize; ++x) {
                TerrainVertex vertex;
                vertex.position = {
                    chunk->worldPosition.x + x * m_vertexSpacing,
                    chunk->heightData[z * m_chunkSize + x],
                    chunk->worldPosition.y + z * m_vertexSpacing
                };
                vertex.texCoord = {
                    static_cast<float>(x) / (m_chunkSize - 1),
                    static_cast<float>(z) / (m_chunkSize - 1)
                };
                vertex.materialWeights = chunk->materialWeights[z * m_chunkSize + x];
                
                // Calculate normals (simplified)
                vertex.normal = {0.0f, 1.0f, 0.0f}; // Will be calculated properly later
                
                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (uint32_t z = 0; z < m_chunkSize - 1; ++z) {
            for (uint32_t x = 0; x < m_chunkSize - 1; ++x) {
                uint32_t topLeft = z * m_chunkSize + x;
                uint32_t topRight = topLeft + 1;
                uint32_t bottomLeft = (z + 1) * m_chunkSize + x;
                uint32_t bottomRight = bottomLeft + 1;

                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Second triangle
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        chunk->vertexCount = static_cast<uint32_t>(vertices.size());
        chunk->indexCount = static_cast<uint32_t>(indices.size());

        // Create vertex buffer
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * chunk->vertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexData = {};
        vertexData.pSysMem = vertices.data();

        m_graphics->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &chunk->vertexBuffer);

        // Create index buffer
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(uint32_t) * chunk->indexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexData = {};
        indexData.pSysMem = indices.data();

        m_graphics->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &chunk->indexBuffer);
    }

    bool TerrainManager::CreateRenderingResources() {
        auto device = m_graphics->GetDevice();
        if (!device) return false;

        // Create shaders (simplified - in practice, load from files)
        const char* vsSource = R"(
            cbuffer TerrainConstants : register(b0) {
                matrix World;
                matrix View;
                matrix Projection;
                float3 ObserverPos;
                float VertexSpacing;
            };

            struct VS_INPUT {
                float3 position : POSITION;
                float3 normal : NORMAL;
                float2 texCoord : TEXCOORD0;
                float4 materialWeights : TEXCOORD1;
            };

            struct VS_OUTPUT {
                float4 position : SV_POSITION;
                float3 worldPos : TEXCOORD0;
                float3 normal : NORMAL;
                float2 texCoord : TEXCOORD1;
                float4 materialWeights : TEXCOORD2;
            };

            VS_OUTPUT main(VS_INPUT input) {
                VS_OUTPUT output;
                
                float4 worldPos = mul(float4(input.position, 1.0), World);
                output.worldPos = worldPos.xyz;
                output.position = mul(mul(worldPos, View), Projection);
                output.normal = normalize(mul(input.normal, (float3x3)World));
                output.texCoord = input.texCoord;
                output.materialWeights = input.materialWeights;
                
                return output;
            }
        )";

        // Compile and create vertex shader (simplified)
        // In practice, use D3DCompile and proper error handling
        
        return true; // Simplified for now
    }

    uint64_t TerrainManager::GetChunkKey(const XMFLOAT2& chunkCoordinate) {
        uint32_t x = static_cast<uint32_t>(chunkCoordinate.x);
        uint32_t z = static_cast<uint32_t>(chunkCoordinate.y);
        return (static_cast<uint64_t>(x) << 32) | z;
    }

    XMFLOAT2 TerrainManager::WorldToChunkCoordinate(const XMFLOAT3& worldPos) {
        float chunkSize = (m_chunkSize - 1) * m_vertexSpacing;
        return {
            std::floor(worldPos.x / chunkSize),
            std::floor(worldPos.z / chunkSize)
        };
    }
}

