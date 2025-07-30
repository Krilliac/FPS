// ─────────────────────────────────────────────────────────────
// MeshHelpers.h  — utility to load a mesh or use a placeholder (Rename this file to MeshHelpers later)
// ─────────────────────────────────────────────────────────────
#pragma once
#include "Utils/Assert.h"
#include "..\Graphics\Mesh.h"
#include <Windows.h>
#include <cstdint>
#include <vector>

// Fallback loader: tries to load |path|, else uses fallback MeshData.
// Any failure triggers an assert in Debug and falls back gracefully.
inline void LoadOrPlaceholderMesh(Mesh& mesh,
    const std::wstring& path,
    const MeshData& fallback)
{
    ASSERT_ALWAYS_MSG(fallback.vertices.size() > 0 && fallback.indices.size() > 0,
        "Fallback MeshData is empty");

    bool loaded = false;
    if (!path.empty())
    {
        loaded = mesh.LoadFromFile(path);
        if (!loaded)
        {
            OutputDebugStringW((L"[Mesh] Failed to load \""
                + path + L"\" – using placeholder mesh.\n").c_str());
        }
    }

    if (!loaded)
    {
        mesh.CreateFromVertices(fallback.vertices, fallback.indices);
        mesh.SetPlaceholder(true);
    }

    ASSERT_ALWAYS_MSG(mesh.GetVertexCount() > 0 && mesh.GetIndexCount() > 0,
        "Mesh ended up with zero vertices or indices!");
}