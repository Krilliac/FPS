#pragma once
#include "..\Core\framework.h"
#include <memory>
#include <vector>

class GraphicsEngine;
class InputManager;
class FPSCamera;
class GameObject;
class Player;
class ProjectilePool;
class Shader;
struct ConstantBuffer;

class Game
{
private:
    GraphicsEngine* m_graphics;
    InputManager* m_input;
    std::unique_ptr<FPSCamera> m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<ProjectilePool> m_projectilePool;
    
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    
    bool m_isPaused;

public:
    Game();
    ~Game();

    HRESULT Initialize(GraphicsEngine* graphics, InputManager* input);
    void Shutdown();
    void Update(float deltaTime);
    void Render();

    // Game state
    void Pause() { m_isPaused = true; }
    void Resume() { m_isPaused = false; }
    bool IsPaused() const { return m_isPaused; }

private:
    void UpdateCamera(float deltaTime);
    void UpdateGameObjects(float deltaTime);
    void HandleInput(float deltaTime);
    void CreateTestObjects();
};
