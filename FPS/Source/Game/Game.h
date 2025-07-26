#pragma once

#include "..\Core\framework.h"        // XMFLOAT3, XMMATRIX, HRESULT
#include <memory>
#include <vector>

// Forward declarations
class GraphicsEngine;
class InputManager;
class FPSCamera;
class GameObject;
class Player;
class ProjectilePool;
class Shader;
struct ConstantBuffer;

// Primitives & placeholder integration
#include "Primitives.h"
#include "PlaceholderMesh.h"

// Scene object types
#include "CubeObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"

class Game
{
public:
    Game();
    ~Game();

    // Initialize engine subsystems and scene
    HRESULT Initialize(GraphicsEngine* graphics, InputManager* input);
    void    Shutdown();

    // Main loop
    void Update(float deltaTime);
    void Render();

    // Pause/resume
    void Pause() { m_isPaused = true; }
    void Resume() { m_isPaused = false; }
    bool IsPaused() const { return m_isPaused; }

private:
    // Helpers
    void UpdateCamera(float deltaTime);
    void UpdateGameObjects(float deltaTime);
    void HandleInput(float deltaTime);
    void CreateTestObjects();

    // Engine & subsystems
    GraphicsEngine* m_graphics{ nullptr };
    InputManager* m_input{ nullptr };
    std::unique_ptr<FPSCamera>               m_camera;
    std::unique_ptr<Shader>                  m_shader;
    std::unique_ptr<Player>                  m_player;
    std::unique_ptr<ProjectilePool>          m_projectilePool;

    // World objects
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    bool m_isPaused{ false };
};