#pragma once
#include "../Core/framework.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <unordered_map>

using Microsoft::WRL::ComPtr;

namespace SparkEngine {
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texCoord;
        DirectX::XMFLOAT3 tangent;
        DirectX::XMFLOAT3 bitangent;
    };

    struct SubMesh {
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t materialIndex = 0;
        DirectX::XMFLOAT3 boundingBoxMin;
        DirectX::XMFLOAT3 boundingBoxMax;
    };

    class Mesh {
    public:
        Mesh(const std::string& name);
        ~Mesh();

        bool LoadFromFile(const std::string& filePath);
        bool CreateFromData(const std::vector<SubMesh>& subMeshes);
        bool CreateBuffers(ID3D11Device* device);

        void Render(ID3D11DeviceContext* context, uint32_t subMeshIndex = 0);
        void RenderAll(ID3D11DeviceContext* context);

        // Accessors
        const std::string& GetName() const { return m_name; }
        size_t GetSubMeshCount() const { return m_subMeshes.size(); }
        const SubMesh& GetSubMesh(size_t index) const { return m_subMeshes[index]; }
        
        // Bounding box
        const DirectX::XMFLOAT3& GetBoundingBoxMin() const { return m_boundingBoxMin; }
        const DirectX::XMFLOAT3& GetBoundingBoxMax() const { return m_boundingBoxMax; }

        // Statistics
        uint32_t GetVertexCount() const;
        uint32_t GetTriangleCount() const;

    private:
        void CalculateTangents(SubMesh& subMesh);
        void CalculateBoundingBox();

        std::string m_name;
        std::vector<SubMesh> m_subMeshes;
        std::vector<ComPtr<ID3D11Buffer>> m_vertexBuffers;
        std::vector<ComPtr<ID3D11Buffer>> m_indexBuffers;
        
        DirectX::XMFLOAT3 m_boundingBoxMin;
        DirectX::XMFLOAT3 m_boundingBoxMax;
    };

    struct Material {
        std::string name;
        DirectX::XMFLOAT4 albedo = {1.0f, 1.0f, 1.0f, 1.0f};
        float metallic = 0.0f;
        float roughness = 0.5f;
        float emissive = 0.0f;
        
        std::string albedoTexture;
        std::string normalTexture;
        std::string metallicRoughnessTexture;
        std::string emissiveTexture;
        std::string occlusionTexture;
    };

    class MeshManager {
    public:
        MeshManager();
        ~MeshManager();

        bool Initialize(ID3D11Device* device);
        void Shutdown();

        // Mesh management
        std::shared_ptr<Mesh> LoadMesh(const std::string& name, const std::string& filePath);
        std::shared_ptr<Mesh> GetMesh(const std::string& name);
        void UnloadMesh(const std::string& name);

        // Procedural mesh generation
        std::shared_ptr<Mesh> CreateCube(const std::string& name, float size = 1.0f);
        std::shared_ptr<Mesh> CreateSphere(const std::string& name, float radius = 1.0f, uint32_t segments = 32);
        std::shared_ptr<Mesh> CreatePlane(const std::string& name, float width = 1.0f, float height = 1.0f);
        std::shared_ptr<Mesh> CreateQuad(const std::string& name);

        // Material management
        std::shared_ptr<Material> LoadMaterial(const std::string& name, const std::string& filePath);
        std::shared_ptr<Material> CreateMaterial(const std::string& name);
        std::shared_ptr<Material> GetMaterial(const std::string& name);

        // Rendering
        void RenderMesh(ID3D11DeviceContext* context, const std::string& meshName, 
                       const DirectX::XMMATRIX& worldMatrix);

        // Statistics
        size_t GetMeshCount() const { return m_meshes.size(); }
        size_t GetMaterialCount() const { return m_materials.size(); }

        // Registry for collaborative systems
        static void RegisterMesh(const std::string& name, std::shared_ptr<Mesh> mesh);
        static std::shared_ptr<Mesh> GetRegisteredMesh(const std::string& name);

    private:
        bool LoadMeshFromFile(const std::string& filePath, std::vector<SubMesh>& outSubMeshes);
        
        ID3D11Device* m_device = nullptr;
        std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
        std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
        
        static std::unordered_map<std::string, std::shared_ptr<Mesh>> s_registeredMeshes;
    };

    // Mesh factory for procedural generation
    class MeshFactory {
    public:
        static std::shared_ptr<Mesh> CreateCube(float size = 1.0f);
        static std::shared_ptr<Mesh> CreateSphere(float radius = 1.0f, uint32_t segments = 32);
        static std::shared_ptr<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f);
        
    private:
        static void CalculateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        static void CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    };
}
