// Game.cpp

#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <DirectXMath.h>

#include "Game.h"

#include "..\Graphics\GraphicsEngine.h"
#include "..\Input\InputManager.h"
#include "..\Camera\FPSCamera.h"
#include "..\Graphics\Shader.h"

#include "GameObject.h"
#include "CubeObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "Player.h"
#include "..\Game\Console.h"
#include "..\Projectiles\ProjectilePool.h"

// Pull in globals defined in FPS.cpp
extern std::unique_ptr<GraphicsEngine> g_graphics;
extern Console                         g_console;

/*-------------------------------------------------------------
  Ctor / Dtor
--------------------------------------------------------------*/
Game::Game() = default;
Game::~Game() { Shutdown(); }

/*-------------------------------------------------------------
  Initialise all game-side systems
--------------------------------------------------------------*/
HRESULT Game::Initialize(GraphicsEngine* graphics,
    InputManager* input)
{
    m_graphics = graphics;
    m_input = input;

    /* Camera ------------------------------------------------*/
    m_camera = std::make_unique<FPSCamera>();
    float aspect = float(m_graphics->GetWindowWidth()) /
        float(m_graphics->GetWindowHeight());
    m_camera->Initialize(aspect);
    m_camera->SetPosition({ 0.0f, 2.0f, -5.0f });

    /* Shaders ----------------------------------------------*/
    m_shader = std::make_unique<Shader>();
    HRESULT hr = m_shader->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    if (FAILED(hr)) return hr;
    if (FAILED(m_shader->LoadVertexShader(L"Shaders\\HLSL\\BasicVS.hlsl")))
        return hr;
    if (FAILED(m_shader->LoadPixelShader(L"Shaders\\HLSL\\BasicPS.hlsl")))
        return hr;

    /* Player -----------------------------------------------*/
    m_player = std::make_unique<Player>();
    hr = m_player->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext(),
        m_camera.get(),
        m_input);
    if (FAILED(hr)) return hr;

    /* Projectile pool --------------------------------------*/
    m_projectilePool =
        std::make_unique<ProjectilePool>(100 /*max*/);
    hr = m_projectilePool->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    if (FAILED(hr)) return hr;

    /* Scene objects ----------------------------------------*/
    CreateTestObjects();
    return S_OK;
}

/*-------------------------------------------------------------
  Tear-down
--------------------------------------------------------------*/
void Game::Shutdown()
{
    m_gameObjects.clear();
    m_projectilePool.reset();
    m_player.reset();
    m_shader.reset();
    m_camera.reset();
}

/*-------------------------------------------------------------
  Per-frame update
--------------------------------------------------------------*/
void Game::Update(float dt)
{
    if (m_isPaused) return;

    HandleInput(dt);
    UpdateCamera(dt);
    UpdateGameObjects(dt);

    if (m_player)         m_player->Update(dt);
    if (m_projectilePool) m_projectilePool->Update(dt);
}

/*-------------------------------------------------------------
  Per-frame render – called between BeginFrame / EndFrame
--------------------------------------------------------------*/
void Game::Render()
{
    // Set shader pair
    m_shader->SetShaders();

    // Build view/projection
    DirectX::XMMATRIX view = m_camera->GetViewMatrix();
    DirectX::XMMATRIX proj = m_camera->GetProjectionMatrix();

    // Draw all scene objects
    ConstantBuffer cb;
    for (auto& obj : m_gameObjects)
    {
        if (!obj->IsActive() || !obj->IsVisible())
            continue;

        cb.World = obj->GetWorldMatrix();
        cb.View = view;
        cb.Projection = proj;
        m_shader->UpdateConstantBuffer(cb);
        obj->Render(view, proj);
    }

    // Draw player-related elements
    m_player->Render(view, proj);
    m_projectilePool->Render(view, proj);

    // Overlay console
    if (g_console.IsVisible())
        g_console.Render(g_graphics->GetContext());
}

/*-------------------------------------------------------------*/
void Game::UpdateCamera(float dt)
{
    if (m_camera) m_camera->Update(dt);
}

/*-------------------------------------------------------------*/
void Game::UpdateGameObjects(float dt)
{
    for (auto& obj : m_gameObjects)
        if (obj && obj->IsActive())
            obj->Update(dt);
}

/*-------------------------------------------------------------
  WASD + mouse look, zoom, and shooting input handling
--------------------------------------------------------------*/
void Game::HandleInput(float dt)
{
    if (!m_input || !m_camera) return;

    // --- Mouse look for yaw/pitch ---
    int dx, dy;
    if (m_input->GetMouseDelta(dx, dy))
    {
        constexpr float mouseSens = 0.005f;
        m_camera->Yaw(dx * mouseSens);
        m_camera->Pitch(-dy * mouseSens);
    }

    // --- Movement (WASD = forward/back & strafe) ---
    float moveSpeed = 10.0f * dt;
    if (m_input->IsKeyDown('W'))        m_camera->MoveForward(moveSpeed);
    if (m_input->IsKeyDown('S'))        m_camera->MoveForward(-moveSpeed);
    if (m_input->IsKeyDown('A'))        m_camera->MoveRight(-moveSpeed);
    if (m_input->IsKeyDown('D'))        m_camera->MoveRight(moveSpeed);
    if (m_input->IsKeyDown(VK_SPACE))   m_camera->MoveUp(moveSpeed);
    if (m_input->IsKeyDown(VK_LCONTROL))m_camera->MoveUp(-moveSpeed);

    // --- Zoom on right-mouse button ---
    if (m_input->IsMouseButtonDown(1))  // right button held
        m_camera->SetZoom(true);
    else
        m_camera->SetZoom(false);

    // --- Shoot on left-mouse click ---
    if (m_input->WasMouseButtonPressed(0) && m_projectilePool)
    {
        auto pos = m_camera->GetPosition();
        auto dir = m_camera->GetForward();
        m_projectilePool->FireProjectile(
            ProjectileType::BULLET,
            pos, dir,
            50.0f
        );
    }
}

/*-------------------------------------------------------------
  Spawn placeholder objects – replace with real assets later
--------------------------------------------------------------*/
void Game::CreateTestObjects()
{
    // Ground plane
    {
        auto ground = std::make_unique<PlaneObject>(20.0f, 20.0f);
        ground->Initialize(
            m_graphics->GetDevice(),
            m_graphics->GetContext());
        ground->SetPosition({ 0.0f, -1.0f, 0.0f });
        m_gameObjects.push_back(std::move(ground));
    }

    // Row of cubes
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>(1.0f);
        cube->Initialize(
            m_graphics->GetDevice(),
            m_graphics->GetContext());
        cube->SetPosition({ i * 3.0f - 6.0f, 1.0f, 10.0f });
        m_gameObjects.push_back(std::move(cube));
    }

    // Single sphere
    {
        auto sphere = std::make_unique<SphereObject>(1.0f, 16, 16);
        sphere->Initialize(
            m_graphics->GetDevice(),
            m_graphics->GetContext());
        sphere->SetPosition({ 5.0f, 0.0f, 0.0f });
        m_gameObjects.push_back(std::move(sphere));
    }
}