#include "Shader.h"

Shader::Shader()
    : m_vertexShader(nullptr)
    , m_pixelShader(nullptr)
    , m_inputLayout(nullptr)
    , m_constantBuffer(nullptr)
    , m_device(nullptr)
    , m_context(nullptr)
{
}

Shader::~Shader()
{
    Shutdown();
}

HRESULT Shader::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    
    HRESULT hr = CreateConstantBuffer();
    if (FAILED(hr)) return hr;
    
    return S_OK;
}

void Shader::Shutdown()
{
    if (m_constantBuffer) { m_constantBuffer->Release(); m_constantBuffer = nullptr; }
    if (m_inputLayout) { m_inputLayout->Release(); m_inputLayout = nullptr; }
    if (m_pixelShader) { m_pixelShader->Release(); m_pixelShader = nullptr; }
    if (m_vertexShader) { m_vertexShader->Release(); m_vertexShader = nullptr; }
}

HRESULT Shader::LoadVertexShader(const std::wstring& filename)
{
    ID3DBlob* vsBlob = nullptr;
    HRESULT hr = CompileShaderFromFile(filename, "main", "vs_5_0", &vsBlob);
    if (FAILED(hr)) return hr;
    
    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
    if (FAILED(hr))
    {
        vsBlob->Release();
        return hr;
    }
    
    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    
    hr = m_device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), 
                                   vsBlob->GetBufferSize(), &m_inputLayout);
    
    vsBlob->Release();
    return hr;
}

HRESULT Shader::LoadPixelShader(const std::wstring& filename)
{
    ID3DBlob* psBlob = nullptr;
    HRESULT hr = CompileShaderFromFile(filename, "main", "ps_5_0", &psBlob);
    if (FAILED(hr)) return hr;
    
    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
    
    psBlob->Release();
    return hr;
}

void Shader::SetShaders()
{
    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->IASetInputLayout(m_inputLayout);
    
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void Shader::UpdateConstantBuffer(const ConstantBuffer& cb)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_context->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        ConstantBuffer* dataPtr = (ConstantBuffer*)mappedResource.pData;
        dataPtr->World = XMMatrixTranspose(cb.World);
        dataPtr->View = XMMatrixTranspose(cb.View);
        dataPtr->Projection = XMMatrixTranspose(cb.Projection);
        m_context->Unmap(m_constantBuffer, 0);
    }
}

HRESULT Shader::CompileShaderFromFile(const std::wstring& filename, const std::string& entryPoint, 
                                    const std::string& shaderModel, ID3DBlob** blobOut)
{
    HRESULT hr = S_OK;
    
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(filename.c_str(), nullptr, nullptr, entryPoint.c_str(), 
                          shaderModel.c_str(), dwShaderFlags, 0, blobOut, &errorBlob);
    
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
    }
    
    if (errorBlob) errorBlob->Release();
    return hr;
}

HRESULT Shader::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    return m_device->CreateBuffer(&bd, nullptr, &m_constantBuffer);
}

