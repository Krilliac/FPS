#pragma once
#include "Primitives.h"
#include "..\Graphics\Mesh.h"

// Attempts to load 'path'; if missing, builds the primitive fallback.
inline void LoadOrPlaceholderMesh(Mesh& mesh,
    const std::wstring& path,
    const MeshData& fallback)
{
    if (!path.empty() && mesh.LoadFromFile(path))
        return;

    // Build CPU‐side vertices from fallback.positions/normals
    std::vector<Vertex> verts;
    verts.reserve(fallback.positions.size());
    for (size_t i = 0; i < fallback.positions.size(); ++i)
    {
        verts.emplace_back(
            fallback.positions[i],
            fallback.normals[i],
            XMFLOAT2(0, 0) // no UVs needed for placeholder
        );
    }
    mesh.CreateFromVertices(verts, fallback.indices);
    mesh.SetPlaceholder(true);
}