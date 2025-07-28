#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <vector>
#include "..\Graphics\Mesh.h"

// Fallback loader: tries to load path, else uses fallback MeshData
inline void LoadOrPlaceholderMesh(Mesh& mesh,
    const std::wstring& path,
    const MeshData& fallback)
{
    if (!path.empty() && mesh.LoadFromFile(path))
        return;

    mesh.CreateFromVertices(fallback.vertices, fallback.indices);
    mesh.SetPlaceholder(true);
}
