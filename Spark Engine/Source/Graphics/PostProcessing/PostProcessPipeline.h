#pragma once
#include "../../Core/framework.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

// ComPtr<using> declaration removed - defined in framework.h

namespace SparkEngine {
    struct PostProcessSettings {
        // Tone mapping
        bool enableToneMapping = true;
        float exposure = 1.0f;
        float gamma = 2.2f;
        
        // Bloom
        bool enableBloom = true;
        float bloomThreshold = 1.0f;
        float bloomIntensity = 0.8f;
        uint32_t bloomPasses = 5;
        
        // SSAO
        bool enableSSAO = true;
        float ssaoRadius = 0.1f;
        float ssaoIntensity = 1.0f;
        uint32_t ssaoSamples = 16;
        
        // Anti-aliasing
        bool enableFXAA = true;
        bool enableTAA = false;
        
        // Color grading
        bool enableColorGrading = false;
        XMFLOAT3 colorBalance = {1.0f, 1.0f, 1.0f};
        float contrast = 1.0f;
        float saturation = 1.0f;
        float brightness = 0.0f;
    };

    class PostProcessEffect {
    public:
        PostProcessEffect(const std::string& name) : m_name(name) {}
        virtual ~PostProcessEffect() = default;

        virtual bool Initialize(ID3D11Device* device) = 0;
        virtual void Shutdown() = 0;
        virtual void Apply(ID3D11DeviceContext* context, 
                          ID3D11ShaderResourceView* input,
                          ID3D11RenderTargetView* output) = 0;

        const std::string& GetName() const { return m_name; }
        void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsEnabled() const { return m_enabled; }

    protected:
        std::string m_name;
        bool m_enabled = true;
    };

    class BloomEffect : public PostProcessEffect {
    public:
        BloomEffect() : PostProcessEffect("Bloom") {}

        bool Initialize(ID3D11Device* device) override;
        void Shutdown() override;
        void Apply(ID3D11DeviceContext* context, 
                  ID3D11ShaderResourceView* input,
                  ID3D11RenderTargetView* output) override;

        void SetThreshold(float threshold) { m_threshold = threshold; }
        void SetIntensity(float intensity) { m_intensity = intensity; }
        void SetPasses(uint32_t passes) { m_passes = std::min(passes, 8u); }

    private:
        struct BloomBuffer {
            ComPtr<ID3D11Texture2D> texture;
            ComPtr<ID3D11RenderTargetView> rtv;
            ComPtr<ID3D11ShaderResourceView> srv;
            uint32_t width, height;
        };

        std::vector<BloomBuffer> m_bloomBuffers;
        ComPtr<ID3D11PixelShader> m_extractPS;
        ComPtr<ID3D11PixelShader> m_blurPS;
        ComPtr<ID3D11PixelShader> m_combinePS;
        ComPtr<ID3D11VertexShader> m_fullscreenVS;
        ComPtr<ID3D11Buffer> m_bloomConstantBuffer;
        
        float m_threshold = 1.0f;
        float m_intensity = 0.8f;
        uint32_t m_passes = 5;

        bool CreateBloomBuffers(ID3D11Device* device, uint32_t width, uint32_t height);
    };

    class SSAOEffect : public PostProcessEffect {
    public:
        SSAOEffect() : PostProcessEffect("SSAO") {}

        bool Initialize(ID3D11Device* device) override;
        void Shutdown() override;
        void Apply(ID3D11DeviceContext* context, 
                  ID3D11ShaderResourceView* input,
                  ID3D11RenderTargetView* output) override;

        void SetRadius(float radius) { m_radius = radius; }
        void SetIntensity(float intensity) { m_intensity = intensity; }
        void SetSampleCount(uint32_t samples) { m_sampleCount = samples; }

    private:
        ComPtr<ID3D11Texture2D> m_ssaoTexture;
        ComPtr<ID3D11RenderTargetView> m_ssaoRTV;
        ComPtr<ID3D11ShaderResourceView> m_ssaoSRV;
        
        ComPtr<ID3D11Texture2D> m_blurTexture;
        ComPtr<ID3D11RenderTargetView> m_blurRTV;
        ComPtr<ID3D11ShaderResourceView> m_blurSRV;
        
        ComPtr<ID3D11Texture2D> m_noiseTexture;
        ComPtr<ID3D11ShaderResourceView> m_noiseSRV;
        
        ComPtr<ID3D11PixelShader> m_ssaoPS;
        ComPtr<ID3D11PixelShader> m_blurPS;
        ComPtr<ID3D11Buffer> m_ssaoConstantBuffer;
        
        std::vector<XMFLOAT4> m_ssaoKernel;
        float m_radius = 0.1f;
        float m_intensity = 1.0f;
        uint32_t m_sampleCount = 16;

        void GenerateSSAOKernel();
        bool CreateNoiseTexture(ID3D11Device* device);
    };

    class ToneMappingEffect : public PostProcessEffect {
    public:
        ToneMappingEffect() : PostProcessEffect("ToneMapping") {}

        bool Initialize(ID3D11Device* device) override;
        void Shutdown() override;
        void Apply(ID3D11DeviceContext* context, 
                  ID3D11ShaderResourceView* input,
                  ID3D11RenderTargetView* output) override;

        void SetExposure(float exposure) { m_exposure = exposure; }
        void SetGamma(float gamma) { m_gamma = gamma; }

    private:
        ComPtr<ID3D11PixelShader> m_toneMappingPS;
        ComPtr<ID3D11Buffer> m_toneMappingConstantBuffer;
        
        float m_exposure = 1.0f;
        float m_gamma = 2.2f;
    };

    class PostProcessPipeline {
    public:
        PostProcessPipeline();
        ~PostProcessPipeline();

        bool Initialize(ID3D11Device* device, uint32_t width, uint32_t height);
        void Shutdown();
        void OnResize(uint32_t width, uint32_t height);

        // Effect management
        void AddEffect(std::unique_ptr<PostProcessEffect> effect);
        void RemoveEffect(const std::string& name);
        PostProcessEffect* GetEffect(const std::string& name);

        // Pipeline execution
        void Process(ID3D11DeviceContext* context, 
                    ID3D11ShaderResourceView* input,
                    ID3D11RenderTargetView* finalOutput);

        // Settings
        void SetSettings(const PostProcessSettings& settings) { m_settings = settings; }
        const PostProcessSettings& GetSettings() const { return m_settings; }

        // Resources
        ID3D11ShaderResourceView* GetFinalResult() const { return m_finalResultSRV.Get(); }

    private:
        struct RenderTarget {
            ComPtr<ID3D11Texture2D> texture;
            ComPtr<ID3D11RenderTargetView> rtv;
            ComPtr<ID3D11ShaderResourceView> srv;
        };

        bool CreateRenderTargets(ID3D11Device* device, uint32_t width, uint32_t height);
        void ReleaseRenderTargets();

        std::vector<std::unique_ptr<PostProcessEffect>> m_effects;
        PostProcessSettings m_settings;
        
        RenderTarget m_tempRenderTarget1;
        RenderTarget m_tempRenderTarget2;
        RenderTarget m_finalResult;
        
        ComPtr<ID3D11VertexShader> m_fullscreenVS;
        ComPtr<ID3D11SamplerState> m_linearSampler;
        ComPtr<ID3D11SamplerState> m_pointSampler;
        
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        ID3D11Device* m_device = nullptr;
    };
}


