#pragma once
#include "../Core/framework.h"
#include "IAsset.h"

namespace SparkEngine {
    class TextureAsset : public IAsset {
    public:
        TextureAsset() = default;
        virtual ~TextureAsset() = default;

        bool Load(const std::string& path) override;
        void Unload() override;

        UINT GetWidth() const { return m_width; }
        UINT GetHeight() const { return m_height; }
        ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

    private:
        ComPtr<ID3D11ShaderResourceView> m_srv;
        UINT m_width = 0, m_height = 0;
    };
}
