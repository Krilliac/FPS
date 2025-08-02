#pragma once
#include "../../Core/framework.h"
#include "../../ECS/Components.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

// ComPtr<using> declaration removed - defined in framework.h

namespace SparkEngine {
    struct DirectionalLightData {
        XMFLOAT3 direction;
        float intensity;
        XMFLOAT3 color;
        float padding;
        XMMATRIX lightViewMatrix;
        XMMATRIX lightProjectionMatrix;
    };

    struct PointLightData {
        XMFLOAT3 position;
        float range;
        XMFLOAT3 color;
        float intensity;
        XMFLOAT3 attenuation; // constant, linear, quadratic
        float padding;
    };

    struct SpotLightData {
        XMFLOAT3 position;
        float range;
        XMFLOAT3 direction;
        float spotAngle;
        XMFLOAT3 color;
        float intensity;
        XMFLOAT3 attenuation;
        float padding;
    };

    struct LightingConstants {
        DirectionalLightData directionalLight;
        PointLightData pointLights[32];
        SpotLightData spotLights[16];
        XMFLOAT3 ambientColor;
        float ambientIntensity;
        uint32_t numPointLights;
        uint32_t numSpotLights;
        uint32_t enableShadows;
        uint32_t shadowMapSize;
    };

    class ShadowMap {
    public:
        ShadowMap();
        ~ShadowMap();

        bool Initialize(ID3D11Device* device, uint32_t width, uint32_t height);
        void Shutdown();

        void BeginRender(ID3D11DeviceContext* context);
        void EndRender(ID3D11DeviceContext* context);

        ID3D11ShaderResourceView* GetShadowMapSRV() const { return m_shadowMapSRV.Get(); }
        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }

    private:
        ComPtr<ID3D11Texture2D> m_shadowMap;
        ComPtr<ID3D11DepthStencilView> m_shadowMapDSV;
        ComPtr<ID3D11ShaderResourceView> m_shadowMapSRV;
        ComPtr<ID3D11RasterizerState> m_shadowRasterizerState;
        
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        D3D11_VIEWPORT m_viewport = {};
    };

    class LightingSystem {
    public:
        LightingSystem();
        ~LightingSystem();

        bool Initialize(ID3D11Device* device);
        void Shutdown();
        void Update(class EntityRegistry* registry);

        // Shadow mapping
        void RenderShadowMaps(ID3D11DeviceContext* context, EntityRegistry* registry);
        void SetupLightingForRender(ID3D11DeviceContext* context);

        // Light management
        void SetAmbientLight(const XMFLOAT3& color, float intensity);
        void EnableShadows(bool enable) { m_shadowsEnabled = enable; }
        bool AreShadowsEnabled() const { return m_shadowsEnabled; }

        // Shadow map settings
        void SetShadowMapSize(uint32_t size);
        uint32_t GetShadowMapSize() const { return m_shadowMapSize; }

        // Access to lighting data for shaders
        const LightingConstants& GetLightingConstants() const { return m_lightingConstants; }
        ID3D11Buffer* GetLightingConstantBuffer() const { return m_lightingConstantBuffer.Get(); }

    private:
        void UpdateLightingConstants(EntityRegistry* registry);
        XMMATRIX CalculateLightViewMatrix(const XMFLOAT3& lightDirection);
        XMMATRIX CalculateLightProjectionMatrix();

        // Shadow map for directional light
        std::unique_ptr<ShadowMap> m_directionalShadowMap;
        
        // Lighting data
        LightingConstants m_lightingConstants = {};
        ComPtr<ID3D11Buffer> m_lightingConstantBuffer;
        
        // Settings
        bool m_shadowsEnabled = true;
        uint32_t m_shadowMapSize = 2048;
        float m_shadowDistance = 100.0f;
        
        ID3D11Device* m_device = nullptr;
    };

    // Deferred lighting renderer
    class DeferredLightingRenderer {
    public:
        DeferredLightingRenderer();
        ~DeferredLightingRenderer();

        bool Initialize(ID3D11Device* device, uint32_t width, uint32_t height);
        void Shutdown();
        void OnResize(uint32_t width, uint32_t height);

        // G-Buffer management
        void BeginGeometryPass(ID3D11DeviceContext* context);
        void EndGeometryPass(ID3D11DeviceContext* context);

        // Lighting pass
        void BeginLightingPass(ID3D11DeviceContext* context);
        void EndLightingPass(ID3D11DeviceContext* context);

        // Post-processing
        void ApplyPostProcessing(ID3D11DeviceContext* context);

        ID3D11ShaderResourceView* GetFinalResult() const { return m_finalResultSRV.Get(); }

    private:
        struct GBuffer {
            ComPtr<ID3D11Texture2D> albedoTexture;          // RGB: Albedo, A: Metallic
            ComPtr<ID3D11Texture2D> normalTexture;          // RGB: World Normal, A: Roughness
            ComPtr<ID3D11Texture2D> motionTexture;          // RG: Motion Vector, B: Depth, A: unused
            ComPtr<ID3D11Texture2D> depthTexture;           // Depth buffer
            
            ComPtr<ID3D11RenderTargetView> albedoRTV;
            ComPtr<ID3D11RenderTargetView> normalRTV;
            ComPtr<ID3D11RenderTargetView> motionRTV;
            ComPtr<ID3D11DepthStencilView> depthDSV;
            
            ComPtr<ID3D11ShaderResourceView> albedoSRV;
            ComPtr<ID3D11ShaderResourceView> normalSRV;
            ComPtr<ID3D11ShaderResourceView> motionSRV;
            ComPtr<ID3D11ShaderResourceView> depthSRV;
        } m_gBuffer;

        ComPtr<ID3D11Texture2D> m_lightingResult;
        ComPtr<ID3D11RenderTargetView> m_lightingResultRTV;
        ComPtr<ID3D11ShaderResourceView> m_lightingResultSRV;

        ComPtr<ID3D11Texture2D> m_finalResult;
        ComPtr<ID3D11RenderTargetView> m_finalResultRTV;
        ComPtr<ID3D11ShaderResourceView> m_finalResultSRV;

        uint32_t m_width = 0;
        uint32_t m_height = 0;
        ID3D11Device* m_device = nullptr;

        bool CreateGBuffer();
        bool CreateLightingBuffers();
        void ReleaseBuffers();
    };
}