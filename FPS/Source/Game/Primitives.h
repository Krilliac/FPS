#pragma once
#include "..\Graphics\Mesh.h"  // Use MeshData from Mesh.h
#include <DirectXMath.h>
using DirectX::XMFLOAT3;

namespace Primitives
{
    MeshData CreateCube(float size = 1.0f);
    MeshData CreatePlane(float width = 1.0f, float depth = 1.0f);
    MeshData CreateSphere(float radius = 0.5f, int slices = 16, int stacks = 16);
}