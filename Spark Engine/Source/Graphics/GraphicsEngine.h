#pragma once
#include "../Core/framework.h"
#include "../Utils/Logger.h"
#include <vector>

namespace SparkEngine {
    class SwapChain;
    class Shader;
    class Mesh;
    class Material;

    class GraphicsEngine {
    public:
        GraphicsEngine();
        ~GraphicsEngine();

        bool Initialize(HWND hwnd, int width, int height);
        void Shutdown();
        void BeginFrame(const Float4& clearColor = {0, 0, 0, 1});
        void EndFrame();

        // Resources
        SwapChain* GetSwapChain() const { return m_swapChain; }
        Shader* CreateShader(const std::wstring& filename);
        Mesh* CreateMesh(const std::string& name);
        Material* CreateMaterial(const std::string& name);

    private:
        bool CreateDeviceAndContext();
        bool CreateSwapChain(HWND hwnd, int width, int height);

        ComPtr<ID3D11Device>           m_device;
        ComPtr<ID3D11DeviceContext>    m_context;
        SwapChain*                     m_swapChain;

        std::vector<std::unique_ptr<Shader>>   m_shaders;
        std::vector<std::unique_ptr<Mesh>>     m_meshes;
        std::vector<std::unique_ptr<Material>> m_materials;
    };
}
