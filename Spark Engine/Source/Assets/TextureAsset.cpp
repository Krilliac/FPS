#include "TextureAsset.h"
#include "../Utils/Logger.h"

namespace SparkEngine {
    bool TextureAsset::Load(const std::string& path) {
        Logger::Info(\"Loading texture: \" + path);
        HRESULT hr = DirectX::CreateWICTextureFromFile(
            nullptr, // use default device passed externally
            std::wstring(path.begin(), path.end()).c_str(),
            nullptr, m_srv.GetAddressOf()
        );
        if (FAILED(hr)) {
            Logger::Error(\"Failed to load texture: \" + path);
            return false;
        }
        // retrieve dimensions
        ComPtr<ID3D11Resource> res;
        m_srv->GetResource(res.GetAddressOf());
        ComPtr<ID3D11Texture2D> tex;
        res.As(&tex);
        D3D11_TEXTURE2D_DESC desc;
        tex->GetDesc(&desc);
        m_width = desc.Width;
        m_height = desc.Height;
        return true;
    }

    void TextureAsset::Unload() {
        Logger::Info(\"Unloading texture\");
        m_srv.Reset();
        m_width = m_height = 0;
    }
}
