#pragma once
#include "../Core/framework.h"
#include "../Core/Engine.h"

namespace SparkEngine {
    class Game {
    public:
        Game();
        ~Game();

        bool Initialize(Engine& engine);
        void Update(float deltaTime);
        void Render();

    private:
        Engine* m_engine = nullptr;
        Entity m_player;
    };
}
