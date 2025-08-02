#pragma once
#include "../Core/framework.h"
#include "../Graphics/GraphicsEngine.h"
#ifdef IMGUI_AVAILABLE
#include <imgui.h>
#endif

namespace SparkEngine {
#ifdef IMGUI_AVAILABLE
    class EditorSystem {
    public:
        EditorSystem(GraphicsEngine& graphics);
        ~EditorSystem();

        bool Initialize();
        void Shutdown();
        void Update();
        void Render();

    private:
        GraphicsEngine& m_graphics;
        bool m_showDemo = false;
    };
#endif
}
