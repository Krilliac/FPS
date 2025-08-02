#pragma once
#include "../Core/framework.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <unordered_map>
#include <memory>

// ComPtr<using> declaration removed - defined in framework.h

namespace SparkEngine {
    enum class ShaderType {
        Vertex,
        Pixel,
        Geometry,
        Hull,
        Domain,
        Compute
    };

    struct ShaderDefine {
        std::string name;
        std::string value;
    };

    struct ShaderCompileInfo {
        std::string filePath;
        std::string entryPoint;
        std::string target;
        std::vector<ShaderDefine> defines;
        UINT compileFlags = 0;
    };

    class Shader {
    public:
        Shader(ShaderType type, const std::string& name);
        virtual ~Shader() = default;

        bool CompileFromFile(ID3D11Device* device, const ShaderCompileInfo& info);
        bool CompileFromSource(ID3D11Device* device, const std::string& source, const ShaderCompileInfo& info);
        
        void Bind(ID3D11DeviceContext* context);
        void Unbind(ID3D11DeviceContext* context);

        const std::string& GetName() const { return m_name; }
        ShaderType GetType() const { return m_type; }
        ID3D11DeviceChild* GetShader() const { return m_shader.Get(); }
        ID3DBlob* GetBytecode() const { return m_bytecode.Get(); }

        // Hot reload support
        void SetSourceFile(const std::string& filePath) { m_sourceFile = filePath; }
        const std::string& GetSourceFile() const { return m_sourceFile; }
        bool NeedsRecompile() const;
        void UpdateFileTime();

    protected:
        virtual HRESULT CreateShaderFromBytecode(ID3D11Device* device, ID3DBlob* bytecode) = 0;

        ShaderType m_type;
        std::string m_name;
        std::string m_sourceFile;
        ComPtr<ID3D11DeviceChild> m_shader;
        ComPtr<ID3DBlob> m_bytecode;
        uint64_t m_lastFileTime = 0;
    };

    class VertexShader : public Shader {
    public:
        VertexShader(const std::string& name) : Shader(ShaderType::Vertex, name) {}
        
        ID3D11VertexShader* GetVertexShader() const { 
            return static_cast<ID3D11VertexShader*>(m_shader.Get()); 
        }

    protected:
        HRESULT CreateShaderFromBytecode(ID3D11Device* device, ID3DBlob* bytecode) override;
    };

    class PixelShader : public Shader {
    public:
        PixelShader(const std::string& name) : Shader(ShaderType::Pixel, name) {}
        
        ID3D11PixelShader* GetPixelShader() const { 
            return static_cast<ID3D11PixelShader*>(m_shader.Get()); 
        }

    protected:
        HRESULT CreateShaderFromBytecode(ID3D11Device* device, ID3DBlob* bytecode) override;
    };

    class ShaderManager {
    public:
        ShaderManager();
        ~ShaderManager();

        bool Initialize(ID3D11Device* device);
        void Shutdown();
        void Update(); // For hot reload

        // Shader management
        std::shared_ptr<VertexShader> LoadVertexShader(const std::string& name, const ShaderCompileInfo& info);
        std::shared_ptr<PixelShader> LoadPixelShader(const std::string& name, const ShaderCompileInfo& info);
        
        std::shared_ptr<Shader> GetShader(const std::string& name);
        bool ReloadShader(const std::string& name);
        void ReloadAllShaders();

        // Shader combinations (programs)
        struct ShaderProgram {
            std::shared_ptr<VertexShader> vertexShader;
            std::shared_ptr<PixelShader> pixelShader;
            ComPtr<ID3D11InputLayout> inputLayout;
            std::string name;
        };

        std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& name,
                                                          const std::string& vsName,
                                                          const std::string& psName,
                                                          const D3D11_INPUT_ELEMENT_DESC* inputElements,
                                                          UINT numElements);

        std::shared_ptr<ShaderProgram> GetShaderProgram(const std::string& name);
        void BindShaderProgram(ID3D11DeviceContext* context, const std::string& name);

        // Hot reload
        void EnableHotReload(bool enable) { m_hotReloadEnabled = enable; }
        bool IsHotReloadEnabled() const { return m_hotReloadEnabled; }

        // Shader directory management
        void AddShaderDirectory(const std::string& directory);
        void SetShaderDirectory(const std::string& directory) { m_shaderDirectory = directory; }

    private:
        HRESULT CompileShaderFromFile(const std::string& filePath, const ShaderCompileInfo& info, ID3DBlob** bytecode);
        HRESULT CompileShaderFromSource(const std::string& source, const ShaderCompileInfo& info, ID3DBlob** bytecode);
        
        void CheckForFileChanges();
        std::string BuildDefineString(const std::vector<ShaderDefine>& defines);

        ID3D11Device* m_device = nullptr;
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
        std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_programs;
        
        std::string m_shaderDirectory = "Spark Engine/Shaders/HLSL/";
        bool m_hotReloadEnabled = true;
        float m_hotReloadCheckInterval = 1.0f; // seconds
        float m_lastHotReloadCheck = 0.0f;
    };
}

