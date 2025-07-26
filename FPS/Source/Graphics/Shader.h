#pragma once
#include "..\Core\framework.h"

struct ConstantBuffer
{
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
};

class Shader
{
private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_inputLayout;
    ID3D11Buffer* m_constantBuffer;
    
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

public:
    Shader();
    ~Shader();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    
    HRESULT LoadVertexShader(const std::wstring& filename);
    HRESULT LoadPixelShader(const std::wstring& filename);
    HRESULT CreateInputLayout();
    
    void SetShaders();
    void UpdateConstantBuffer(const ConstantBuffer& cb);
    
    // Utility methods
    static HRESULT CompileShaderFromFile(const std::wstring& filename, const std::string& entryPoint, 
                                       const std::string& shaderModel, ID3DBlob** blobOut);

private:
    HRESULT CreateConstantBuffer();
};
