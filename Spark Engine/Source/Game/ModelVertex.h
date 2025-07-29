#pragma once
#include <DirectXMath.h>

struct ModelVertex {
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT2 TexCoord;

    ModelVertex()
        : Position(0.f, 0.f, 0.f)
        , Normal(0.f, 0.f, 0.f)
        , TexCoord(0.f, 0.f) {
    }

    ModelVertex(const DirectX::XMFLOAT3& pos,
        const DirectX::XMFLOAT3& norm,
        const DirectX::XMFLOAT2& uv)
        : Position(pos)
        , Normal(norm)
        , TexCoord(uv) {
    }
};