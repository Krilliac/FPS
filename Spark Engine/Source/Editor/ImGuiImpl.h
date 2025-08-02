#pragma once
#include "../Core/framework.h"
#include "../Graphics/GraphicsEngine.h"

namespace SparkEngine {
    struct ImGuiImpl {
        static void Init(GraphicsEngine& graphics);
        static void Shutdown();
        static void NewFrame();
        static void Render();
    };
}
