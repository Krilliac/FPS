#pragma once
#include "../../Core/framework.h"
#include "../Texture.h"
#include <memory>
#include <string>

namespace SparkEngine {
    struct PBRMaterialData {
        XMFLOAT3 albedo = {1.0f, 1.0f, 1.0f};
        float metallic = 0.0f;
        float roughness = 0.5f;
        float ao = 1.0f;
        XMFLOAT3 emissive = {0.0f, 0.0f, 0.0f};
        float normalStrength = 1.0f;
        float alphaCutoff = 0.5f;
        float padding[3] = {0.0f, 0.0f, 0.0f};  // Align to 16 bytes
    };

    class PBRMaterial {
    private:
        PBRMaterialData m_materialData;
        std::shared_ptr<Texture> m_albedoMap;
        std::shared_ptr<Texture> m_normalMap;
        std::shared_ptr<Texture> m_metallicMap;
        std::shared_ptr<Texture> m_roughnessMap;
        std::shared_ptr<Texture> m_aoMap;
        std::shared_ptr<Texture> m_emissiveMap;
        std::shared_ptr<Texture> m_heightMap;

        ComPtr<ID3D11Buffer> m_materialBuffer;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        std::string m_name;
        bool m_isDirty = true;

    public:
        PBRMaterial(const std::string& name = "PBRMaterial");
        ~PBRMaterial() = default;

        bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
        void Shutdown();

        // Texture setters
        void SetAlbedoMap(std::shared_ptr<Texture> texture) { m_albedoMap = texture; }
        void SetNormalMap(std::shared_ptr<Texture> texture) { m_normalMap = texture; }
        void SetMetallicMap(std::shared_ptr<Texture> texture) { m_metallicMap = texture; }
        void SetRoughnessMap(std::shared_ptr<Texture> texture) { m_roughnessMap = texture; }
        void SetAOMap(std::shared_ptr<Texture> texture) { m_aoMap = texture; }
        void SetEmissiveMap(std::shared_ptr<Texture> texture) { m_emissiveMap = texture; }
        void SetHeightMap(std::shared_ptr<Texture> texture) { m_heightMap = texture; }

        // Texture getters
        std::shared_ptr<Texture> GetAlbedoMap() const { return m_albedoMap; }
        std::shared_ptr<Texture> GetNormalMap() const { return m_normalMap; }
        std::shared_ptr<Texture> GetMetallicMap() const { return m_metallicMap; }
        std::shared_ptr<Texture> GetRoughnessMap() const { return m_roughnessMap; }
        std::shared_ptr<Texture> GetAOMap() const { return m_aoMap; }
        std::shared_ptr<Texture> GetEmissiveMap() const { return m_emissiveMap; }
        std::shared_ptr<Texture> GetHeightMap() const { return m_heightMap; }

        // Material data accessors
        const PBRMaterialData& GetMaterialData() const { return m_materialData; }
        PBRMaterialData& GetMaterialData() { m_isDirty = true; return m_materialData; }

        // Material property setters
        void SetAlbedo(const XMFLOAT3& albedo) { m_materialData.albedo = albedo; m_isDirty = true; }
        void SetMetallic(float metallic) { m_materialData.metallic = metallic; m_isDirty = true; }
        void SetRoughness(float roughness) { m_materialData.roughness = roughness; m_isDirty = true; }
        void SetAO(float ao) { m_materialData.ao = ao; m_isDirty = true; }
        void SetEmissive(const XMFLOAT3& emissive) { m_materialData.emissive = emissive; m_isDirty = true; }
        void SetNormalStrength(float strength) { m_materialData.normalStrength = strength; m_isDirty = true; }

        // Material operations
        void BindTextures(ID3D11DeviceContext* context, UINT startSlot = 0);
        void UpdateMaterialBuffer();
        void BindMaterialBuffer(UINT slot = 2);

        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }

        bool LoadFromFile(const std::string& filePath);
        bool SaveToFile(const std::string& filePath) const;
    };

    // Material Manager for caching and managing materials
    class MaterialManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<PBRMaterial>> m_materials;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

    public:
        bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
        void Shutdown();

        std::shared_ptr<PBRMaterial> CreateMaterial(const std::string& name);
        std::shared_ptr<PBRMaterial> LoadMaterial(const std::string& filePath);
        std::shared_ptr<PBRMaterial> GetMaterial(const std::string& name);

        void RemoveMaterial(const std::string& name);
        void ClearAll();

        const auto& GetMaterials() const { return m_materials; }
    };
}
