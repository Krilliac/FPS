// Shader.cpp
#include "Shader.h"
#include "Utils/Assert.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <filesystem>
#include <iostream>

Shader::Shader() {
    std::wcout << L"[INFO] Shader constructed." << std::endl;
}

Shader::~Shader() {
    std::wcout << L"[INFO] Shader destructor called." << std::endl;
    Shutdown();
}

HRESULT Shader::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) {
    std::wcout << L"[OPERATION] Shader::Initialize called." << std::endl;
    ASSERT_MSG(device != nullptr, "Shader::Initialize device is null");
    ASSERT_MSG(context != nullptr, "Shader::Initialize context is null");

    m_device = device;
    m_context = context;

    HRESULT hr = CreateConstantBuffer();
    std::wcout << L"[INFO] Shader constant buffer creation HR=0x" << std::hex << hr << std::dec << std::endl;
    ASSERT_MSG(SUCCEEDED(hr), "CreateConstantBuffer failed");
    return hr;
}

void Shader::Shutdown() {
    std::wcout << L"[OPERATION] Shader::Shutdown called." << std::endl;
    if (m_constantBuffer) { m_constantBuffer->Release(); m_constantBuffer = nullptr; }
    if (m_inputLayout) { m_inputLayout->Release();    m_inputLayout = nullptr; }
    if (m_pixelShader) { m_pixelShader->Release();    m_pixelShader = nullptr; }
    if (m_vertexShader) { m_vertexShader->Release();   m_vertexShader = nullptr; }
    std::wcout << L"[INFO] Shader shutdown complete." << std::endl;
}

HRESULT Shader::LoadVertexShader(const std::wstring& filename) {
    std::wcout << L"[OPERATION] Shader::LoadVertexShader called. filename=" << filename << std::endl;
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
    std::wcout << L"[INFO] Vertex shader loaded." << std::endl;
    return hr;
}

HRESULT Shader::LoadPixelShader(const std::wstring& filename) {
    std::wcout << L"[OPERATION] Shader::LoadPixelShader called. filename=" << filename << std::endl;
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
    std::wcout << L"[INFO] Pixel shader loaded." << std::endl;
    return hr;
}

void Shader::SetShaders() {
    std::wcout << L"[OPERATION] Shader::SetShaders called." << std::endl;
    ASSERT(m_context != nullptr);
    ASSERT(m_vertexShader != nullptr);
    ASSERT(m_pixelShader != nullptr);
    ASSERT(m_inputLayout != nullptr);
    ASSERT(m_constantBuffer != nullptr);

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->IASetInputLayout(m_inputLayout);
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

    std::wcout << L"[INFO] Shaders set and constant buffer bound." << std::endl;
}

void Shader::UpdateConstantBuffer(const ConstantBuffer& cb) {
    std::wcout << L"[OPERATION] Shader::UpdateConstantBuffer called." << std::endl;
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
    std::wcout << L"[INFO] Constant buffer updated." << std::endl;
}

HRESULT Shader::CompileShaderFromFile(const std::wstring& filename,
    const std::string& entryPoint,
    const std::string& shaderModel,
    ID3DBlob** blobOut)
{
    std::wcout << L"[OPERATION] Shader::CompileShaderFromFile called. filename=" << filename << std::endl;
    ASSERT_MSG(!filename.empty(), "CompileShaderFromFile: filename empty");
    ASSERT(blobOut != nullptr);

    // Try multiple shader locations based on your actual directory structure
    std::vector<std::wstring> searchPaths = {
        filename,                                           // Relative to working dir
        std::wstring(L"Spark Engine/Shaders/HLSL/") + filename.substr(filename.find_last_of(L"/\\") + 1),  // Your actual location
        std::wstring(L"Spark Engine\\Shaders\\HLSL\\") + filename.substr(filename.find_last_of(L"/\\") + 1), // Windows paths
        std::wstring(L"Shaders/HLSL/") + filename.substr(filename.find_last_of(L"/\\") + 1),       // Expected location
        std::wstring(L"Shaders\\HLSL\\") + filename.substr(filename.find_last_of(L"/\\") + 1),     // Windows expected
        std::wstring(L"../Shaders/HLSL/") + filename.substr(filename.find_last_of(L"/\\") + 1),    // One level up
        std::wstring(L"build/bin/Debug/Shaders/HLSL/") + filename.substr(filename.find_last_of(L"/\\") + 1) // Build output
    };

    std::wstring foundPath;
    bool fileFound = false;

    for (const auto& path : searchPaths) {
        if (std::filesystem::exists(path)) {
            foundPath = path;
            fileFound = true;
            std::wstring msg = L"Found shader at: " + foundPath + L"\n";
            OutputDebugStringW(msg.c_str());
            break;
        }
    }

    if (!fileFound) {
        std::wstring errorMsg = L"Shader file not found in any search path: " + filename + L"\n";
        errorMsg += L"Searched paths:\n";
        for (const auto& path : searchPaths) {
            errorMsg += L"  - " + path + L" (exists: " + (std::filesystem::exists(path) ? L"YES" : L"NO") + L")\n";
        }
        OutputDebugStringW(errorMsg.c_str());

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
        foundPath.c_str(),  // Use the found path
        nullptr,
        nullptr,
        entryPoint.c_str(),
        shaderModel.c_str(),
        flags,
        0,
        blobOut,
        &errorBlob);

    if (FAILED(hr)) {
        std::wstring errorMsg = L"D3DCompileFromFile failed for: " + foundPath + L"\n";
        errorMsg += L"HRESULT: 0x" + std::to_wstring(hr) + L"\n";
        OutputDebugStringW(errorMsg.c_str());

        if (errorBlob) {
            OutputDebugStringA("Shader compilation errors:\n");
            OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
            OutputDebugStringA("\n");
        }
    }

    if (errorBlob) errorBlob->Release();

    std::wcout << L"[INFO] Shader compiled from file." << std::endl;
    return hr;
}

HRESULT Shader::CreateConstantBuffer() {
    std::wcout << L"[OPERATION] Shader::CreateConstantBuffer called." << std::endl;
    ASSERT_MSG(m_device != nullptr, "CreateConstantBuffer: device is null");

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&bd, nullptr, &m_constantBuffer);
    ASSERT_MSG(SUCCEEDED(hr), "CreateBuffer (constant) failed");
    std::wcout << L"[INFO] Constant buffer created." << std::endl;
    return hr;
}