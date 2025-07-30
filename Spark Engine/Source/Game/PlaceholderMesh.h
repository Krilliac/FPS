#pragma once

#include "Utils/Assert.h"
#include "..\Graphics\Mesh.h"
#include <Windows.h>
#include <iostream>

// Attempts to initialize |mesh| with D3D device/context, then load from |path|.
// On failure, generates a procedural cube placeholder. Includes verbose debug logs.
inline void LoadOrPlaceholderMesh(
    Mesh& mesh,
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    const std::wstring& path)
{
    // 1) Initialize mesh with device/context
    std::wcerr << L"[DEBUG] Initializing mesh with D3D device/context..." << std::endl;
    HRESULT hrInit = mesh.Initialize(device, context);
    std::wcerr << L"[DEBUG] Mesh::Initialize returned HR = 0x"
        << std::hex << hrInit << std::dec << std::endl;
    ASSERT_MSG(SUCCEEDED(hrInit), "Mesh::Initialize failed");

    // 2) Attempt to load from file
    bool loaded = false;
    if (!path.empty())
    {
        std::wcerr << L"[DEBUG] Attempting mesh.LoadFromFile(\"" << path << L"\")..." << std::endl;
        loaded = mesh.LoadFromFile(path);
        std::wcerr << L"[DEBUG] LoadFromFile returned: "
            << (loaded ? L"SUCCESS" : L"FAILURE") << std::endl;
        if (!loaded)
            OutputDebugStringW((L"[Mesh] Failed to load \"" + path +
                L"\" – falling back to procedural cube.\n").c_str());
    }
    else
    {
        std::wcerr << L"[DEBUG] No path provided, skipping LoadFromFile." << std::endl;
    }

    // 3) Fallback to procedural cube if load failed
    if (!loaded)
    {
        std::wcerr << L"[DEBUG] Creating procedural cube placeholder..." << std::endl;
        HRESULT hrCube = mesh.CreateCube(1.0f);
        std::wcerr << L"[DEBUG] CreateCube(1.0f) HR = 0x"
            << std::hex << hrCube << std::dec << std::endl;
    }

    // 4) Log final mesh counts
    UINT vc = mesh.GetVertexCount();
    UINT ic = mesh.GetIndexCount();
    std::wcerr << L"[DEBUG] Final mesh counts: Vertices=" << vc
        << L", Indices=" << ic << std::endl;

    // 5) Assert success
    ASSERT_ALWAYS_MSG(vc > 0 && ic > 0,
        "Mesh ended up with zero vertices or indices after placeholder creation!");
}