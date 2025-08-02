#include "Game.h"
#include "../Components.h"

namespace SparkEngine {
    Game::Game() {}
    Game::~Game() {}

    bool Game::Initialize(Engine& engine) {
        m_engine = &engine;
        auto& registry = engine.GetRegistry();

        // Create player entity
        m_player = registry.CreateEntity();
        registry.AddComponent<TransformComponent>(m_player, Float3{0, 0, 0});
        registry.AddComponent<RenderComponent>(m_player, "PlayerMesh", "PlayerMaterial");
        return true;
    }

    void Game::Update(float deltaTime) {
        // Rotate player
        auto& transform = m_engine->GetRegistry().GetComponent<TransformComponent>(m_player);
        transform.rotation.y += deltaTime;
    }

    void Game::Render() {
        auto& graphics = m_engine->GetGraphics();
        // For each entity with RenderComponent and TransformComponent:
        // graphics.DrawMesh(...);
    }
}
