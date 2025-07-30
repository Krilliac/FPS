// Shader.cpp
#include "Shader.h"
#include "Utils/Assert.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <filesystem>

Shader::Shader()
{
}

Shader::~Shader()
{
    Shutdown();
}

HRESULT Shader::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT_MSG(device != nullptr, "Shader::Initialize device is null");
    ASSERT_MSG(context != nullptr, "Shader::Initialize context is null");

    m_device = device;
    m_context = context;

    HRESULT hr = CreateConstantBuffer();
    ASSERT_MSG(SUCCEEDED(hr), "CreateConstantBuffer failed");
    return hr;
}

void Shader::Shutdown()
{
    if (m_constantBuffer) { m_constantBuffer->Release(); m_constantBuffer = nullptr; }
    if (m_inputLayout) { m_inputLayout->Release();    m_inputLayout = nullptr; }
    if (m_pixelShader) { m_pixelShader->Release();    m_pixelShader = nullptr; }
    if (m_vertexShader) { m_vertexShader->Release();   m_vertexShader = nullptr; }
}

HRESULT Shader::LoadVertexShader(const std::wstring& filename)
{
    ASSERT_MSG(!filename.empty(), "LoadVertexShader: filename empty");
    ASSERT_MSG(m_device != nullptr, "LoadVertexShader: device is null");

    ID3DBlob* vsBlob = nullptr;
    HRESULT hr = CompileShaderFromFile(filename, "main", "vs_5_0", &vsBlob);
    ASSERT_MSG(SUCCEEDED(hr), "CompileShaderFromFile (VS) failed");
    if (FAILED(hr)) return hr;

    hr = m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader);
    ASSERT_MSG(SUCCEEDED(hr), "CreateVertexShader failed");
    if (FAILED(hr)) { vsBlob->Release(); return hr; }

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_device->CreateInputLayout(
        layoutDesc,
        ARRAYSIZE(layoutDesc),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout);
    ASSERT_MSG(SUCCEEDED(hr), "CreateInputLayout failed");

    vsBlob->Release();
    return hr;
}

HRESULT Shader::LoadPixelShader(const std::wstring& filename)
{
    ASSERT_MSG(!filename.empty(), "LoadPixelShader: filename empty");
    ASSERT_MSG(m_device != nullptr, "LoadPixelShader: device is null");

    ID3DBlob* psBlob = nullptr;
    HRESULT hr = CompileShaderFromFile(filename, "main", "ps_5_0", &psBlob);
    ASSERT_MSG(SUCCEEDED(hr), "CompileShaderFromFile (PS) failed");
    if (FAILED(hr)) return hr;

    hr = m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader);
    ASSERT_MSG(SUCCEEDED(hr), "CreatePixelShader failed");

    psBlob->Release();
    return hr;
}

void Shader::SetShaders()
{
    ASSERT(m_context != nullptr);
    ASSERT(m_vertexShader != nullptr);
    ASSERT(m_pixelShader != nullptr);
    ASSERT(m_inputLayout != nullptr);
    ASSERT(m_constantBuffer != nullptr);

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->IASetInputLayout(m_inputLayout);
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void Shader::UpdateConstantBuffer(const ConstantBuffer& cb)
{
    ASSERT(m_context != nullptr);
    ASSERT(m_constantBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = m_context->Map(
        m_constantBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped);
    ASSERT_MSG(SUCCEEDED(hr), "VS Map constant buffer failed");
    if (FAILED(hr)) return;

    auto dataPtr = reinterpret_cast<ConstantBuffer*>(mapped.pData);
    dataPtr->World = XMMatrixTranspose(cb.World);
    dataPtr->View = XMMatrixTranspose(cb.View);
    dataPtr->Projection = XMMatrixTranspose(cb.Projection);

    m_context->Unmap(m_constantBuffer, 0);
}

HRESULT Shader::CompileShaderFromFile(const std::wstring& filename,
    const std::string& entryPoint,
    const std::string& shaderModel,
    ID3DBlob** blobOut)
{
    ASSERT_MSG(!filename.empty(), "CompileShaderFromFile: filename empty");
    ASSERT(blobOut != nullptr);

    // Check if file exists first
    if (!std::filesystem::exists(filename)) {
        std::wstring errorMsg = L"Shader file not found: " + filename;
        OutputDebugStringW(errorMsg.c_str());
        OutputDebugStringW(L"\n");

        // Also print current working directory for debugging
        wchar_t currentDir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, currentDir);
        std::wstring cwdMsg = L"Current working directory: ";
        cwdMsg += currentDir;
        cwdMsg += L"\n";
        OutputDebugStringW(cwdMsg.c_str());

        return E_FAIL;
    }

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        nullptr,
        nullptr,
        entryPoint.c_str(),
        shaderModel.c_str(),
        flags,
        0,
        blobOut,
        &errorBlob);

    if (FAILED(hr)) {
        std::wstring errorMsg = L"D3DCompileFromFile failed for: " + filename + L"\n";
        errorMsg += L"HRESULT: 0x" + std::to_wstring(hr) + L"\n";
        OutputDebugStringW(errorMsg.c_str());

        if (errorBlob) {
            OutputDebugStringA("Shader compilation errors:\n");
            OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
            OutputDebugStringA("\n");
        }
    }

    if (errorBlob) errorBlob->Release();

    ASSERT_MSG(SUCCEEDED(hr), "D3DCompileFromFile failed");
    return hr;
}

HRESULT Shader::CreateConstantBuffer()
{
    ASSERT_MSG(m_device != nullptr, "CreateConstantBuffer: device is null");

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&bd, nullptr, &m_constantBuffer);
    ASSERT_MSG(SUCCEEDED(hr), "CreateBuffer (constant) failed");
    return hr;
}