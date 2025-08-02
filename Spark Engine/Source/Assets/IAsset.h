#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class IAsset {
    public:
        virtual ~IAsset() = default;
        virtual bool Load(const std::string& path) = 0;
        virtual void Unload() = 0;
    };
}
