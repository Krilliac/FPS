/**
 * @file MaterialSystem.h
 * @brief Advanced material system for AAA-quality rendering with PBR support
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class provides a comprehensive material system supporting physically-based
 * rendering (PBR), material variants, texture streaming, and extensive console
 * integration for real-time material debugging and authoring.
 */

#pragma once

#include "Utils/Assert.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <chrono>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

/**
 * @brief Material blend modes
 */
enum class BlendMode
{
    Opaque,            ///< Fully opaque material
    AlphaTest,         ///< Alpha testing (cutout)
    Transparent,       ///< Alpha blending
    Additive,          ///< Additive blending
    Multiply,          ///< Multiplicative blending
    Screen            ///< Screen blending
};

/**
 * @brief Material cull modes
 */
enum class CullMode
{
    None,             ///< No culling (double-sided)
    Front,            ///< Front face culling
    Back              ///< Back face culling (default)
};

/**
 * @brief Texture types for material slots
 */
enum class MaterialTextureType
{
    Albedo,           ///< Base color/albedo texture
    Normal,           ///< Normal map texture
    Metallic,         ///< Metallic map texture
    Roughness,        ///< Roughness map texture
    Occlusion,        ///< Ambient occlusion texture
    Emissive,         ///< Emissive texture
    Height,           ///< Height/displacement map
    DetailAlbedo,     ///< Detail albedo texture
    DetailNormal,     ///< Detail normal map
    Subsurface,       ///< Subsurface scattering map
    Transmission,     ///< Transmission map
    Clearcoat,        ///< Clearcoat layer map
    ClearcoatRoughness, ///< Clearcoat roughness map
    Anisotropy,       ///< Anisotropy direction map
    Custom0,          ///< Custom texture slot 0
    Custom1,          ///< Custom texture slot 1
    Custom2,          ///< Custom texture slot 2
    Custom3           ///< Custom texture slot 3
};

/**
 * @brief Texture sampling parameters
 */
struct TextureSampling
{
    D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC;  ///< Texture filtering mode
    D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP; ///< U address mode
    D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP; ///< V address mode
    D3D11_TEXTURE_ADDRESS_MODE addressW = D3D11_TEXTURE_ADDRESS_WRAP; ///< W address mode
    UINT maxAnisotropy = 16;                          ///< Maximum anisotropy level
    float mipLODBias = 0.0f;                         ///< MIP level bias
    float minLOD = 0.0f;                             ///< Minimum LOD
    float maxLOD = D3D11_FLOAT32_MAX;                ///< Maximum LOD
    XMFLOAT4 borderColor = {0, 0, 0, 0};             ///< Border color for border address mode
};

/**
 * @brief Material texture slot
 */
struct MaterialTexture
{
    ComPtr<ID3D11ShaderResourceView> texture;        ///< DirectX texture resource
    TextureSampling sampling;                         ///< Sampling parameters
    XMFLOAT2 tiling = {1.0f, 1.0f};                 ///< UV tiling
    XMFLOAT2 offset = {0.0f, 0.0f};                 ///< UV offset
    float intensity = 1.0f;                          ///< Texture intensity/strength
    bool enabled = false;                            ///< Whether this texture slot is active
    std::string filePath;                            ///< Original file path for hot-reloading
};

/**
 * @brief PBR material properties
 */
struct PBRProperties
{
    XMFLOAT4 albedoColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< Base albedo color
    float metallicFactor = 0.0f;                      ///< Metallic factor (0 = dielectric, 1 = metallic)
    float roughnessFactor = 0.5f;                     ///< Roughness factor (0 = mirror, 1 = fully rough)
    float normalScale = 1.0f;                         ///< Normal map intensity
    float occlusionStrength = 1.0f;                   ///< Ambient occlusion strength
    XMFLOAT3 emissiveColor = {0.0f, 0.0f, 0.0f};     ///< Emissive color
    float emissiveFactor = 0.0f;                      ///< Emissive intensity
    float alphaCutoff = 0.5f;                         ///< Alpha cutoff for alpha testing
    float indexOfRefraction = 1.5f;                   ///< Index of refraction for dielectrics
};

/**
 * @brief Advanced material properties
 */
struct AdvancedProperties
{
    // Subsurface scattering
    bool subsurfaceEnabled = false;                   ///< Enable subsurface scattering
    XMFLOAT3 subsurfaceColor = {1.0f, 1.0f, 1.0f};  ///< Subsurface scattering color
    float subsurfaceRadius = 1.0f;                   ///< Subsurface scattering radius
    
    // Clearcoat
    bool clearcoatEnabled = false;                    ///< Enable clearcoat layer
    float clearcoatFactor = 0.0f;                     ///< Clearcoat layer strength
    float clearcoatRoughness = 0.0f;                  ///< Clearcoat layer roughness
    
    // Anisotropy
    bool anisotropyEnabled = false;                   ///< Enable anisotropic reflections
    float anisotropyFactor = 0.0f;                    ///< Anisotropy strength
    XMFLOAT2 anisotropyDirection = {1.0f, 0.0f};     ///< Anisotropy direction
    
    // Transmission
    bool transmissionEnabled = false;                 ///< Enable transmission
    float transmissionFactor = 0.0f;                  ///< Transmission strength
    XMFLOAT3 transmissionColor = {1.0f, 1.0f, 1.0f}; ///< Transmission color
    
    // Sheen (fabric-like materials)
    bool sheenEnabled = false;                        ///< Enable sheen
    XMFLOAT3 sheenColor = {0.0f, 0.0f, 0.0f};       ///< Sheen color
    float sheenRoughness = 0.0f;                      ///< Sheen roughness
    
    // Iridescence
    bool iridescenceEnabled = false;                  ///< Enable iridescence
    float iridescenceFactor = 0.0f;                   ///< Iridescence strength
    float iridescenceIOR = 1.3f;                      ///< Iridescence IOR
    float iridescenceThickness = 100.0f;              ///< Iridescence thickness (nm)
};

/**
 * @brief Material render state
 */
struct MaterialRenderState
{
    BlendMode blendMode = BlendMode::Opaque;          ///< Blending mode
    CullMode cullMode = CullMode::Back;               ///< Face culling mode
    bool depthTest = true;                            ///< Enable depth testing
    bool depthWrite = true;                           ///< Enable depth writing
    bool castShadows = true;                          ///< Cast shadows
    bool receiveShadows = true;                       ///< Receive shadows
    int renderQueue = 2000;                           ///< Render queue priority
    bool doubleSided = false;                         ///< Double-sided rendering
};

/**
 * @brief Material definition
 */
class Material
{
public:
    Material(const std::string& name);
    ~Material() = default;

    // Getters
    const std::string& GetName() const { return m_name; }
    const PBRProperties& GetPBRProperties() const { return m_pbrProperties; }
    const AdvancedProperties& GetAdvancedProperties() const { return m_advancedProperties; }
    const MaterialRenderState& GetRenderState() const { return m_renderState; }
    const MaterialTexture& GetTexture(MaterialTextureType type) const;
    const std::string& GetActiveVariant() const;
    std::vector<std::string> GetAvailableVariants() const;

    // Setters
    void SetPBRProperties(const PBRProperties& properties) { m_pbrProperties = properties; }
    void SetAdvancedProperties(const AdvancedProperties& properties) { m_advancedProperties = properties; }
    void SetRenderState(const MaterialRenderState& state) { m_renderState = state; }
    void SetTexture(MaterialTextureType type, const MaterialTexture& texture);
    
    // Texture management
    bool LoadTexture(MaterialTextureType type, const std::string& filePath, ID3D11Device* device);
    void UnloadTexture(MaterialTextureType type);
    bool HasTexture(MaterialTextureType type) const;
    
    // Shader parameter binding
    void BindToShader(ID3D11DeviceContext* context) const;
    
    // Material variants
    void CreateVariant(const std::string& variantName, const std::vector<std::string>& defines);
    void SetActiveVariant(const std::string& variantName);
    
    // Serialization
    bool SaveToFile(const std::string& filePath) const;
    bool LoadFromFile(const std::string& filePath, ID3D11Device* device);
    
    // Console integration
    std::string GetDetailedInfo() const;
    void Console_SetProperty(const std::string& property, float value);
    void Console_SetColor(const std::string& property, float r, float g, float b);
    void Console_ReloadTextures(ID3D11Device* device);

private:
    std::string m_name;
    PBRProperties m_pbrProperties;
    AdvancedProperties m_advancedProperties;
    MaterialRenderState m_renderState;
    std::unordered_map<MaterialTextureType, MaterialTexture> m_textures;
    std::unordered_map<std::string, std::vector<std::string>> m_variants;
    std::string m_activeVariant;
};

/**
 * @brief Material system manager
 */
class MaterialSystem
{
public:
    /**
     * @brief Material system performance metrics
     */
    struct MaterialMetrics
    {
        int loadedMaterials;        ///< Number of loaded materials
        int textureCount;           ///< Total number of loaded textures
        size_t textureMemory;       ///< Texture memory usage in bytes
        int materialSwitches;       ///< Material switches per frame
        int textureBinds;           ///< Texture binds per frame
        float averageLoadTime;      ///< Average material load time
        int failedLoads;            ///< Number of failed material loads
        bool hotReloadEnabled;      ///< Hot reload status
        int variantCount;           ///< Number of material variants
    };

    MaterialSystem();
    ~MaterialSystem();

    /**
     * @brief Initialize the material system
     */
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    /**
     * @brief Shutdown the material system
     */
    void Shutdown();

    // Material management
    std::shared_ptr<Material> CreateMaterial(const std::string& name);
    std::shared_ptr<Material> LoadMaterial(const std::string& filePath);
    std::shared_ptr<Material> GetMaterial(const std::string& name) const;
    void UnloadMaterial(const std::string& name);
    void UnloadAllMaterials();

    // Default materials
    std::shared_ptr<Material> GetDefaultMaterial() const { return m_defaultMaterial; }
    std::shared_ptr<Material> GetErrorMaterial() const { return m_errorMaterial; }

    // Texture management
    ComPtr<ID3D11ShaderResourceView> LoadTexture(const std::string& filePath);
    void UnloadTexture(const std::string& filePath);
    ComPtr<ID3D11SamplerState> GetSampler(const TextureSampling& sampling);

    // Hot reloading
    void EnableHotReload(bool enabled) { m_hotReloadEnabled = enabled; }
    void EnableHotReloading(bool enabled);
    void UpdateHotReload();
    int ReloadAllMaterials();

    // Frame management
    void BeginFrame();
    void EndFrame();

    // ========================================================================
    // CONSOLE INTEGRATION METHODS
    // ========================================================================

    /**
     * @brief Get material system metrics
     */
    MaterialMetrics Console_GetMetrics() const;

    /**
     * @brief List all loaded materials
     */
    std::string Console_ListMaterials() const;

    /**
     * @brief Get detailed material information
     */
    std::string Console_GetMaterialInfo(const std::string& materialName) const;

    /**
     * @brief Reload specific material
     */
    bool Console_ReloadMaterial(const std::string& materialName);

    /**
     * @brief Reload all materials
     */
    int Console_ReloadAllMaterials();

    /**
     * @brief Create material variant
     */
    bool Console_CreateVariant(const std::string& materialName, const std::string& variantName, const std::vector<std::string>& defines);

    /**
     * @brief Set material property via console
     */
    void Console_SetMaterialProperty(const std::string& materialName, const std::string& property, float value);

    /**
     * @brief Set material color property via console
     */
    void Console_SetMaterialColor(const std::string& materialName, const std::string& property, float r, float g, float b);

    /**
     * @brief Enable/disable hot reload
     */
    void Console_SetHotReload(bool enabled);

    /**
     * @brief Clear material cache
     */
    void Console_ClearCache();

    /**
     * @brief Force garbage collection of unused materials
     */
    void Console_GarbageCollect();

    /**
     * @brief Set texture quality settings
     */
    void Console_SetTextureQuality(const std::string& quality);

    /**
     * @brief Get texture memory usage
     */
    std::string Console_GetTextureMemoryInfo() const;

    /**
     * @brief Validate all loaded materials
     */
    int Console_ValidateMaterials();

    /**
     * @brief Dump detailed material information
     */
    std::string Console_DumpMaterialDetails(const std::string& materialName) const;

    /**
     * @brief Export material to file
     */
    bool Console_ExportMaterial(const std::string& materialName, const std::string& filePath);

    /**
     * @brief Import material from file
     */
    bool Console_ImportMaterial(const std::string& filePath);

    /**
     * @brief List all available texture types
     */
    std::string Console_ListTextureTypes() const;

    /**
     * @brief Load texture to specific material slot
     */
    bool Console_LoadTextureToSlot(const std::string& materialName, 
                                  const std::string& textureType, 
                                  const std::string& texturePath);

    /**
     * @brief Unload texture from material slot
     */
    void Console_UnloadTextureFromSlot(const std::string& materialName, const std::string& textureType);

    /**
     * @brief List material variants
     */
    std::string Console_ListMaterialVariants(const std::string& materialName) const;

private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    // Material storage
    std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
    std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textureCache;
    std::unordered_map<size_t, ComPtr<ID3D11SamplerState>> m_samplerCache;

    // Default materials
    std::shared_ptr<Material> m_defaultMaterial;
    std::shared_ptr<Material> m_errorMaterial;

    // Hot reloading
    bool m_hotReloadEnabled;
    std::unordered_map<std::string, uint64_t> m_fileTimestamps;

    // Performance tracking
    mutable std::mutex m_metricsMutex;
    MaterialMetrics m_metrics;
    std::chrono::high_resolution_clock::time_point m_frameStartTime;

    // Helper methods
    HRESULT CreateDefaultMaterials();
    HRESULT CreateSampler(const TextureSampling& sampling, ID3D11SamplerState** sampler);
    size_t HashSampling(const TextureSampling& sampling) const;
    uint64_t GetFileTimestamp(const std::string& filePath) const;
    ComPtr<ID3D11ShaderResourceView> LoadTextureFromFile(const std::string& filePath);
    void UpdateMetrics();
    void PerformPeriodicMaintenance();
    std::string TextureTypeToString(MaterialTextureType type) const;
    MaterialTextureType StringToTextureType(const std::string& str) const;
};