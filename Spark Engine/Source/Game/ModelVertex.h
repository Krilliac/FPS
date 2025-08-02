// ModelVertex.h
#pragma once
#include "Utils/Assert.h"
#include <DirectXMath.h>
#include <cmath>               // add this for std::isfinite

struct ModelVertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;

    ModelVertex() noexcept
        : Position(0.f, 0.f, 0.f)
        , Normal(0.f, 0.f, 0.f)
        , TexCoord(0.f, 0.f) {
    }

    ModelVertex(const XMFLOAT3& pos,
        const XMFLOAT3& norm,
        const XMFLOAT2& uv) noexcept
        : Position(pos)
        , Normal(norm)
        , TexCoord(uv)
    {
        // Validate incoming data
        SPARK_ASSERT(std::isfinite(pos.x) && std::isfinite(pos.y) && std::isfinite(pos.z));
        SPARK_ASSERT(std::isfinite(norm.x) && std::isfinite(norm.y) && std::isfinite(norm.z));
        SPARK_ASSERT(std::isfinite(uv.x) && std::isfinite(uv.y));
    }
};

