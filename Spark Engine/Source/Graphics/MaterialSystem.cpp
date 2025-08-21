/**
 * @file MaterialSystem.cpp
 * @brief Implementation of advanced material system for AAA-quality rendering
 * @author Spark Engine Team
 * @date 2025
 */

#include "MaterialSystem.h"
#include "Utils/Assert.h"
#include "../Utils/SparkConsole.h"
#include <iostream>
#include <fstream>
#include <sstream>

// ============================================================================
// MATERIAL CLASS IMPLEMENTATION
// ============================================================================

Material::Material(const std::string& name)
    : m_name(name)
{
    // Initialize with default PBR properties
    m_pbrProperties = {};
    m_advancedProperties = {};
    m_renderState = {};
}

const MaterialTexture& Material::GetTexture(MaterialTextureType type) const
{
    auto it = m_textures.find(type);
    if (it != m_textures.end()) {
        return it->second;
    }
    
    // Return empty texture
    static MaterialTexture emptyTexture;
    return emptyTexture;
}

void Material::SetTexture(MaterialTextureType type, const MaterialTexture& texture)
{
    m_textures[type] = texture;
}

bool Material::LoadTexture(MaterialTextureType type, const std::string& filePath, ID3D11Device* device)
{
    // Implementation would load texture from file
    MaterialTexture texture;
    texture.filePath = filePath;
    texture.enabled = true;
    
    // TODO: Actual texture loading implementation
    
    m_textures[type] = texture;
    return true;
}

void Material::UnloadTexture(MaterialTextureType type)
{
    auto it = m_textures.find(type);
    if (it != m_textures.end()) {
        m_textures.erase(it);
    }
}

bool Material::HasTexture(MaterialTextureType type) const
{
    return m_textures.find(type) != m_textures.end();
}

void Material::BindToShader(ID3D11DeviceContext* context) const
{
    // Implementation would bind material properties to shader constant buffers
}

void Material::CreateVariant(const std::string& variantName, const std::vector<std::string>& defines)
{
    m_variants[variantName] = defines;
}

void Material::SetActiveVariant(const std::string& variantName)
{
    if (m_variants.find(variantName) != m_variants.end()) {
        m_activeVariant = variantName;
    }
}

bool Material::SaveToFile(const std::string& filePath) const
{
    // Implementation would serialize material to file
    return true;
}

bool Material::LoadFromFile(const std::string& filePath, ID3D11Device* device)
{
    // Implementation would deserialize material from file
    return true;
}

std::string Material::GetDetailedInfo() const
{
    std::stringstream ss;
    ss << "Material: " << m_name << "\n";
    ss << "Albedo: (" << m_pbrProperties.albedoColor.x << ", " << m_pbrProperties.albedoColor.y << ", " << m_pbrProperties.albedoColor.z << ")\n";
    ss << "Metallic: " << m_pbrProperties.metallicFactor << "\n";
    ss << "Roughness: " << m_pbrProperties.roughnessFactor << "\n";
    ss << "Textures: " << m_textures.size() << "\n";
    return ss.str();
}

void Material::Console_SetProperty(const std::string& property, float value)
{
    if (property == "metallic") {
        m_pbrProperties.metallicFactor = value;
    } else if (property == "roughness") {
        m_pbrProperties.roughnessFactor = value;
    } else if (property == "normal") {
        m_pbrProperties.normalScale = value;
    }
}

void Material::Console_SetColor(const std::string& property, float r, float g, float b)
{
    if (property == "albedo") {
        m_pbrProperties.albedoColor = {r, g, b, m_pbrProperties.albedoColor.w};
    } else if (property == "emissive") {
        m_pbrProperties.emissiveColor = {r, g, b};
    }
}

void Material::Console_ReloadTextures(ID3D11Device* device)
{
    // Implementation would reload all textures
}

// ============================================================================
// MATERIAL SYSTEM IMPLEMENTATION
// ============================================================================

MaterialSystem::MaterialSystem()
    : m_device(nullptr)
    , m_context(nullptr)
    , m_hotReloadEnabled(false)
{
}

MaterialSystem::~MaterialSystem()
{
    Shutdown();
}

HRESULT MaterialSystem::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT(device && context);
    
    m_device = device;
    m_context = context;
    
    // Create default materials
    HRESULT hr = CreateDefaultMaterials();
    if (FAILED(hr)) {
        return hr;
    }
    
    Spark::SimpleConsole::GetInstance().Log("MaterialSystem initialized successfully", "SUCCESS");
    return S_OK;
}

void MaterialSystem::Shutdown()
{
    m_materials.clear();
    m_textureCache.clear();
    m_samplerCache.clear();
    m_defaultMaterial.reset();
    m_errorMaterial.reset();
    
    m_device = nullptr;
    m_context = nullptr;
}

std::shared_ptr<Material> MaterialSystem::CreateMaterial(const std::string& name)
{
    auto material = std::make_shared<Material>(name);
    m_materials[name] = material;
    return material;
}

std::shared_ptr<Material> MaterialSystem::LoadMaterial(const std::string& filePath)
{
    // Implementation would load material from file
    auto material = std::make_shared<Material>(filePath);
    m_materials[filePath] = material;
    return material;
}

std::shared_ptr<Material> MaterialSystem::GetMaterial(const std::string& name) const
{
    auto it = m_materials.find(name);
    return (it != m_materials.end()) ? it->second : m_defaultMaterial;
}

void MaterialSystem::UnloadMaterial(const std::string& name)
{
    m_materials.erase(name);
}

void MaterialSystem::UnloadAllMaterials()
{
    m_materials.clear();
}

ComPtr<ID3D11ShaderResourceView> MaterialSystem::LoadTexture(const std::string& filePath)
{
    auto it = m_textureCache.find(filePath);
    if (it != m_textureCache.end()) {
        return it->second;
    }
    
    // Implementation would load texture using DirectX
    ComPtr<ID3D11ShaderResourceView> texture;
    // ... texture loading code ...
    
    m_textureCache[filePath] = texture;
    return texture;
}

void MaterialSystem::UnloadTexture(const std::string& filePath)
{
    m_textureCache.erase(filePath);
}

ComPtr<ID3D11SamplerState> MaterialSystem::GetSampler(const TextureSampling& sampling)
{
    size_t hash = HashSampling(sampling);
    auto it = m_samplerCache.find(hash);
    if (it != m_samplerCache.end()) {
        return it->second;
    }
    
    ComPtr<ID3D11SamplerState> sampler;
    HRESULT hr = CreateSampler(sampling, &sampler);
    if (SUCCEEDED(hr)) {
        m_samplerCache[hash] = sampler;
    }
    
    return sampler;
}

void MaterialSystem::UpdateHotReload()
{
    if (!m_hotReloadEnabled) return;
    
    // Implementation would check file timestamps and reload changed materials
}

int MaterialSystem::ReloadAllMaterials()
{
    int reloaded = 0;
    for (auto& pair : m_materials) {
        // Implementation would reload each material
        reloaded++;
    }
    return reloaded;
}

void MaterialSystem::BeginFrame()
{
    m_frameStartTime = std::chrono::high_resolution_clock::now();
    UpdateMetrics();
}

void MaterialSystem::EndFrame()
{
    // Reset per-frame counters
    m_metrics.materialSwitches = 0;
    m_metrics.textureBinds = 0;
}

// ============================================================================
// CONSOLE INTEGRATION METHODS
// ============================================================================

MaterialSystem::MaterialMetrics MaterialSystem::Console_GetMetrics() const
{
    std::lock_guard<std::mutex> lock(m_metricsMutex);
    return m_metrics;
}

std::string MaterialSystem::Console_ListMaterials() const
{
    std::stringstream ss;
    ss << "=== Loaded Materials ===\n";
    for (const auto& pair : m_materials) {
        ss << pair.first << "\n";
    }
    ss << "Total: " << m_materials.size() << " materials";
    return ss.str();
}

std::string MaterialSystem::Console_GetMaterialInfo(const std::string& materialName) const
{
    auto material = GetMaterial(materialName);
    if (material) {
        return material->GetDetailedInfo();
    }
    return "Material not found: " + materialName;
}

bool MaterialSystem::Console_ReloadMaterial(const std::string& materialName)
{
    auto it = m_materials.find(materialName);
    if (it != m_materials.end()) {
        // Implementation would reload the material
        return true;
    }
    return false;
}

int MaterialSystem::Console_ReloadAllMaterials()
{
    return ReloadAllMaterials();
}

bool MaterialSystem::Console_CreateVariant(const std::string& materialName, const std::string& variantName, const std::vector<std::string>& defines)
{
    auto material = GetMaterial(materialName);
    if (material) {
        material->CreateVariant(variantName, defines);
        return true;
    }
    return false;
}

void MaterialSystem::Console_SetMaterialProperty(const std::string& materialName, const std::string& property, float value)
{
    auto material = GetMaterial(materialName);
    if (material) {
        material->Console_SetProperty(property, value);
    }
}

void MaterialSystem::Console_SetMaterialColor(const std::string& materialName, const std::string& property, float r, float g, float b)
{
    auto material = GetMaterial(materialName);
    if (material) {
        material->Console_SetColor(property, r, g, b);
    }
}

void MaterialSystem::Console_SetHotReload(bool enabled)
{
    m_hotReloadEnabled = enabled;
}

void MaterialSystem::Console_ClearCache()
{
    m_textureCache.clear();
    m_samplerCache.clear();
}

void MaterialSystem::Console_GarbageCollect()
{
    // Implementation would remove unused materials and textures
}

void MaterialSystem::Console_SetTextureQuality(const std::string& quality)
{
    // Implementation would adjust texture quality settings
}

std::string MaterialSystem::Console_GetTextureMemoryInfo() const
{
    std::stringstream ss;
    ss << "Texture cache: " << m_textureCache.size() << " textures\n";
    ss << "Sampler cache: " << m_samplerCache.size() << " samplers";
    return ss.str();
}

int MaterialSystem::Console_ValidateMaterials()
{
    int validCount = 0;
    for (const auto& pair : m_materials) {
        // Implementation would validate each material
        validCount++;
    }
    return validCount;
}

// ============================================================================
// PRIVATE HELPER METHODS
// ============================================================================

HRESULT MaterialSystem::CreateDefaultMaterials()
{
    // Create default material
    m_defaultMaterial = std::make_shared<Material>("Default");
    
    // Create error material (bright magenta for missing materials)
    m_errorMaterial = std::make_shared<Material>("Error");
    auto errorProps = m_errorMaterial->GetPBRProperties();
    errorProps.albedoColor = {1.0f, 0.0f, 1.0f, 1.0f}; // Magenta
    m_errorMaterial->SetPBRProperties(errorProps);
    
    return S_OK;
}

HRESULT MaterialSystem::CreateSampler(const TextureSampling& sampling, ID3D11SamplerState** sampler)
{
    if (!m_device) return E_FAIL;
    
    D3D11_SAMPLER_DESC desc = {};
    desc.Filter = sampling.filter;
    desc.AddressU = sampling.addressU;
    desc.AddressV = sampling.addressV;
    desc.AddressW = sampling.addressW;
    desc.MaxAnisotropy = sampling.maxAnisotropy;
    desc.MipLODBias = sampling.mipLODBias;
    desc.MinLOD = sampling.minLOD;
    desc.MaxLOD = sampling.maxLOD;
    desc.BorderColor[0] = sampling.borderColor.x;
    desc.BorderColor[1] = sampling.borderColor.y;
    desc.BorderColor[2] = sampling.borderColor.z;
    desc.BorderColor[3] = sampling.borderColor.w;
    
    return m_device->CreateSamplerState(&desc, sampler);
}

size_t MaterialSystem::HashSampling(const TextureSampling& sampling) const
{
    // Simple hash of sampling parameters
    size_t hash = 0;
    hash ^= std::hash<int>{}(static_cast<int>(sampling.filter));
    hash ^= std::hash<int>{}(static_cast<int>(sampling.addressU)) << 1;
    hash ^= std::hash<int>{}(static_cast<int>(sampling.addressV)) << 2;
    hash ^= std::hash<int>{}(static_cast<int>(sampling.addressW)) << 3;
    return hash;
}

uint64_t MaterialSystem::GetFileTimestamp(const std::string& filePath) const
{
    // Implementation would get file modification timestamp
    return 0;
}

ComPtr<ID3D11ShaderResourceView> MaterialSystem::LoadTextureFromFile(const std::string& filePath)
{
    // Implementation would load texture using DirectX texture loading
    ComPtr<ID3D11ShaderResourceView> texture;
    return texture;
}

void MaterialSystem::UpdateMetrics()
{
    std::lock_guard<std::mutex> lock(m_metricsMutex);
    m_metrics.loadedMaterials = static_cast<int>(m_materials.size());
    m_metrics.textureCount = static_cast<int>(m_textureCache.size());
    m_metrics.variantCount = 0; // Would count all variants across all materials
    m_metrics.hotReloadEnabled = m_hotReloadEnabled;
}

std::string MaterialSystem::TextureTypeToString(MaterialTextureType type) const
{
    switch (type) {
        case MaterialTextureType::Albedo: return "Albedo";
        case MaterialTextureType::Normal: return "Normal";
        case MaterialTextureType::Metallic: return "Metallic";
        case MaterialTextureType::Roughness: return "Roughness";
        case MaterialTextureType::Occlusion: return "Occlusion";
        case MaterialTextureType::Emissive: return "Emissive";
        case MaterialTextureType::Height: return "Height";
        case MaterialTextureType::DetailAlbedo: return "DetailAlbedo";
        case MaterialTextureType::DetailNormal: return "DetailNormal";
        case MaterialTextureType::Subsurface: return "Subsurface";
        case MaterialTextureType::Transmission: return "Transmission";
        case MaterialTextureType::Clearcoat: return "Clearcoat";
        case MaterialTextureType::ClearcoatRoughness: return "ClearcoatRoughness";
        case MaterialTextureType::Anisotropy: return "Anisotropy";
        case MaterialTextureType::Custom0: return "Custom0";
        case MaterialTextureType::Custom1: return "Custom1";
        case MaterialTextureType::Custom2: return "Custom2";
        case MaterialTextureType::Custom3: return "Custom3";
        default: return "Unknown";
    }
}

MaterialTextureType MaterialSystem::StringToTextureType(const std::string& str) const
{
    if (str == "Albedo") return MaterialTextureType::Albedo;
    if (str == "Normal") return MaterialTextureType::Normal;
    if (str == "Metallic") return MaterialTextureType::Metallic;
    if (str == "Roughness") return MaterialTextureType::Roughness;
    if (str == "Occlusion") return MaterialTextureType::Occlusion;
    if (str == "Emissive") return MaterialTextureType::Emissive;
    if (str == "Height") return MaterialTextureType::Height;
    if (str == "DetailAlbedo") return MaterialTextureType::DetailAlbedo;
    if (str == "DetailNormal") return MaterialTextureType::DetailNormal;
    if (str == "Subsurface") return MaterialTextureType::Subsurface;
    if (str == "Transmission") return MaterialTextureType::Transmission;
    if (str == "Clearcoat") return MaterialTextureType::Clearcoat;
    if (str == "ClearcoatRoughness") return MaterialTextureType::ClearcoatRoughness;
    if (str == "Anisotropy") return MaterialTextureType::Anisotropy;
    if (str == "Custom0") return MaterialTextureType::Custom0;
    if (str == "Custom1") return MaterialTextureType::Custom1;
    if (str == "Custom2") return MaterialTextureType::Custom2;
    if (str == "Custom3") return MaterialTextureType::Custom3;
    return MaterialTextureType::Albedo; // Default
}