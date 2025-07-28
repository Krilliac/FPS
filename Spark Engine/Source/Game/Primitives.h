#pragma once

#include "..\Graphics\Mesh.h"    // MeshData
#include <DirectXMath.h>
using DirectX::XMFLOAT3;

// Procedural primitive mesh generators
namespace Primitives
{
    // Returns a MeshData containing a cube of the given size
    MeshData CreateCube(float size = 1.0f);

    // Returns a flat plane
    MeshData CreatePlane(float width = 1.0f, float depth = 1.0f);

    // Returns a UV sphere
    MeshData CreateSphere(float radius = 0.5f, int slices = 16, int stacks = 16);
}
