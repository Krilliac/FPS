#pragma once
#include <vector>
#include <DirectXMath.h>
using DirectX::XMFLOAT3;

struct MeshData
{
    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<uint32_t> indices;
};

namespace Primitives
{
    MeshData CreateCube(float size = 1.0f);
    MeshData CreatePlane(float width = 1.0f, float depth = 1.0f);
    MeshData CreateSphere(float radius = 0.5f, int slices = 16, int stacks = 16);
}