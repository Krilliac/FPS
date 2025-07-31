#include "RenderCommandQueue.h"

namespace SparkEngine {
    bool RenderThreadManager::Initialize(ID3D11Device* device) {
        m_device = device;
        m_device->CreateDeferredContext(0, &m_deferredContext);
        m_running = true;
        m_renderThread = std::make_unique<std::thread>(&RenderThreadManager::RenderThreadFunction, this);
        return true;
    }

    void RenderThreadManager::Shutdown() {
        m_commandQueue.Shutdown();
        m_running = false;
        if (m_renderThread && m_renderThread->joinable()) m_renderThread->join();
    }

    void RenderThreadManager::RenderThreadFunction() {
        while (m_running) m_commandQueue.WaitAndExecute();
    }
}