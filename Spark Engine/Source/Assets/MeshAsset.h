#pragma once
#include "../Core/framework.h"
#include "IAsset.h"

namespace SparkEngine {
    class MeshAsset : public IAsset {
    public:
        MeshAsset() = default;
        virtual ~MeshAsset() = default;

        bool Load(const std::string& path) override;
        void Unload() override;

        ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }
        ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }
        UINT GetIndexCount() const { return m_indexCount; }

    private:
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        UINT m_indexCount = 0;
    };
}
