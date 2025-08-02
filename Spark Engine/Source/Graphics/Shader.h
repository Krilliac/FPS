#pragma once
#include "../Core/framework.h"

// Light buffer structure
struct LightBuffer
{
    XMFLOAT3 LightDirection;
    float LightIntensity;
    XMFLOAT3 LightColor;
    float AmbientIntensity;
    XMFLOAT3 CameraPosition;
    float SpecularPower;
};

// Constant buffer structure  
struct ConstantBuffer
{
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
};

class Shader
{
public:
    Shader();
    ~Shader();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    HRESULT LoadVertexShader(const std::wstring& filename);
    HRESULT LoadPixelShader(const std::wstring& filename);
    void SetShaders();
    void UpdateConstantBuffer(const ConstantBuffer& cb);
    void UpdateLightBuffer(const LightBuffer& lb);
    void Shutdown();

private:
    HRESULT CreateConstantBuffer();
    HRESULT CreateLightBuffer();

    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_inputLayout;
    ID3D11Buffer* m_constantBuffer;
    ID3D11Buffer* m_lightBuffer;
};
