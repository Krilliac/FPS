#include "MeshAsset.h"
#include "../Utils/Logger.h"
#include <fstream>

namespace SparkEngine {
    bool MeshAsset::Load(const std::string& path) {
        Logger::Info(\"Loading mesh: \" + path);
        // very basic OBJ loader or custom format...
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            Logger::Error(\"Failed to open mesh file: \" + path);
            return false;
        }
        // pseudo code: read vertex/index data...
        // create D3D buffers...
        return true;
    }

    void MeshAsset::Unload() {
        Logger::Info(\"Unloading mesh\");
        m_vertexBuffer.Reset();
        m_indexBuffer.Reset();
        m_indexCount = 0;
    }
}
