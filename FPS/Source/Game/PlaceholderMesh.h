#pragma once

#include "..\Graphics\Mesh.h"
#include <DirectXMath.h>
using DirectX::XMFLOAT2;

// Forward declare MeshData since it's already in Mesh.h
struct MeshData;

// Attempts to LoadFromFile(path); if that fails, builds fallback primitive.
inline void LoadOrPlaceholderMesh(Mesh& mesh,
    const std::wstring& path,
    const MeshData& fallback)
{
    if (!path.empty() && mesh.LoadFromFile(path))
        return;

    // MeshData already contains proper Vertex objects
    mesh.CreateFromVertices(fallback.vertices, fallback.indices);
    mesh.SetPlaceholder(true);
}