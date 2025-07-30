// Shader.h
#pragma once

#include "Utils/Assert.h"
#include "..\Core\framework.h"      // ConstantBuffer definition
#include <d3d11.h>
#include <string>

// Constant buffer structure
struct ConstantBuffer
{
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
};

class Shader
{
public:
    Shader();
    ~Shader();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void    Shutdown();

    HRESULT LoadVertexShader(const std::wstring& filename);
    HRESULT LoadPixelShader(const std::wstring& filename);

    void SetShaders();
    void UpdateConstantBuffer(const ConstantBuffer& cb);

    static HRESULT CompileShaderFromFile(const std::wstring& filename,
        const std::string& entryPoint,
        const std::string& shaderModel,
        ID3DBlob** blobOut);

private:
    HRESULT CreateConstantBuffer();

    ID3D11Device* m_device{ nullptr };
    ID3D11DeviceContext* m_context{ nullptr };
    ID3D11VertexShader* m_vertexShader{ nullptr };
    ID3D11PixelShader* m_pixelShader{ nullptr };
    ID3D11InputLayout* m_inputLayout{ nullptr };
    ID3D11Buffer* m_constantBuffer{ nullptr };
};