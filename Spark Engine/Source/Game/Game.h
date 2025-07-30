#pragma once

#include "..\Core\framework.h"    // XMFLOAT3, XMMATRIX, HRESULT
#include "Utils/Assert.h"
#include <memory>
#include <vector>

// Forward declarations
class GraphicsEngine;
class InputManager;
class SparkEngineCamera;
class GameObject;
class Player;
class ProjectilePool;
class Shader;
struct ConstantBuffer;

#include "Primitives.h"
#include "PlaceholderMesh.h"
#include "CubeObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"

class Game
{
public:
    Game();
    ~Game();

    // Engine & scene initialization / teardown
    HRESULT Initialize(GraphicsEngine* graphics, InputManager* input);
    void    Shutdown();

    // Main loop hooks
    void Update(float deltaTime);
    void Render();

    // Pause helpers
    void Pause() { m_isPaused = true; }
    void Resume() { m_isPaused = false; }
    bool IsPaused() const { return m_isPaused; }

private:
    // Internal helpers
    void UpdateCamera(float dt);
    void UpdateGameObjects(float dt);
    void HandleInput(float dt);
    void CreateTestObjects();

    // Engine-side pointers (not owned)
    GraphicsEngine* m_graphics{ nullptr };
    InputManager* m_input{ nullptr };

    // Sub-systems owned by Game
    std::unique_ptr<SparkEngineCamera> m_camera;
    std::unique_ptr<Shader>            m_shader;
    std::unique_ptr<Player>            m_player;
    std::unique_ptr<ProjectilePool>    m_projectilePool;

    // Scene objects
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    bool m_isPaused{ false };
};