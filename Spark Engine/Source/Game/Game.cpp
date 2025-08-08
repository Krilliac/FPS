#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <DirectXMath.h>

#include "Game.h"
#include "Utils/Assert.h"

#include "..\Graphics\GraphicsEngine.h"
#include "..\Input\InputManager.h"
#include "..\Camera\SparkEngineCamera.h"
#include "..\Graphics\Shader.h"
#include "GameObject.h"
#include "CubeObject.h"
#include "PlaneObject.h"
#include "SphereObject.h"
#include "Player.h"
#include "..\Game\Console.h"
#include "..\Projectiles\ProjectilePool.h"
#include <iostream>
#include "..\Scene\SceneManager.h"

// Pull in globals defined in SparkEngine.cpp
extern std::unique_ptr<GraphicsEngine> g_graphics;
extern Console                         g_console;

/*-------------------------------------------------------------
  Ctor / Dtor
--------------------------------------------------------------*/
Game::Game() = default;
Game::~Game()
{
    Shutdown();
}

/*-------------------------------------------------------------
  Initialise all game-side systems
--------------------------------------------------------------*/
HRESULT Game::Initialize(GraphicsEngine* graphics,
    InputManager* input)
{
    ASSERT(graphics != nullptr);
    ASSERT(input != nullptr);

    m_graphics = graphics;
    m_input = input;

    // SceneManager setup
    m_sceneManager = std::make_unique<SceneManager>(graphics, input);
    bool sceneLoaded = m_sceneManager->LoadScene(L"Assets/Scenes/level1.scene");
    std::wcerr << L"[Game] SceneManager::LoadScene returned: " << (sceneLoaded ? L"SUCCESS" : L"FAILURE") << std::endl;

    /* Camera ------------------------------------------------*/
    m_camera = std::make_unique<SparkEngineCamera>();
    ASSERT(m_camera);

    float aspect = float(m_graphics->GetWindowWidth()) /
        float(m_graphics->GetWindowHeight());
    ASSERT_MSG(aspect > 0.0f, "Invalid aspect ratio");

    m_camera->Initialize(aspect);
    m_camera->SetPosition({ 0.0f, 2.0f, -5.0f });

    /* Shaders ----------------------------------------------*/
    m_shader = std::make_unique<Shader>();
    ASSERT(m_shader);

    HRESULT hr = m_shader->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    ASSERT_MSG(SUCCEEDED(hr), "Shader::Initialize failed");
    if (FAILED(hr)) return hr;

    hr = m_shader->LoadVertexShader(L"Shaders\\HLSL\\BasicVS.hlsl");
    ASSERT_MSG(SUCCEEDED(hr), "LoadVertexShader failed");
    if (FAILED(hr)) return hr;

    hr = m_shader->LoadPixelShader(L"Shaders\\HLSL\\BasicPS.hlsl");
    ASSERT_MSG(SUCCEEDED(hr), "LoadPixelShader failed");
    if (FAILED(hr)) return hr;

    /* Player -----------------------------------------------*/
    m_player = std::make_unique<Player>();
    ASSERT(m_player);

    hr = m_player->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext(),
        m_camera.get(),
        m_input);
    ASSERT_MSG(SUCCEEDED(hr), "Player::Initialize failed");
    if (FAILED(hr)) return hr;

    /* Projectile pool --------------------------------------*/
    m_projectilePool = std::make_unique<ProjectilePool>(100);
    ASSERT(m_projectilePool);

    hr = m_projectilePool->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    ASSERT_MSG(SUCCEEDED(hr), "ProjectilePool::Initialize failed");
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
    ASSERT(m_shader != nullptr);
    m_shader->SetShaders();

    XMMATRIX view = m_camera->GetViewMatrix();
    XMMATRIX proj = m_camera->GetProjectionMatrix();

    int renderedCount = 0;
    int skippedCount = 0;
    // Render objects from SceneManager
    if (m_sceneManager) {
        for (auto& obj : m_sceneManager->GetObjects()) {
            if (!obj || !obj->IsActive() || !obj->IsVisible()) {
                ++skippedCount;
                std::wcerr << L"[Game] Skipping SceneManager object in Render: active=" << (obj ? obj->IsActive() : false) << L" visible=" << (obj ? obj->IsVisible() : false) << std::endl;
                continue;
            }
            ConstantBuffer cb{};
            cb.World = obj->GetWorldMatrix();
            cb.View = view;
            cb.Projection = proj;
            m_shader->UpdateConstantBuffer(cb);
            obj->Render(view, proj);
            ++renderedCount;
        }
    }

    static int frameNum = 0;
    if (++frameNum % 60 == 0)  // once per 60 frames
    {
        std::wcerr << L"[DEBUG] Rendered " << renderedCount << L" objects, skipped " << skippedCount << L" this frame\n";
    }

    if (m_player)         m_player->Render(view, proj);
    if (m_projectilePool) m_projectilePool->Render(view, proj);

    if (g_console.IsVisible())
        g_console.Render(g_graphics->GetContext());
}

/*-------------------------------------------------------------*/
void Game::UpdateCamera(float dt)
{
    ASSERT(dt >= 0.0f);
    if (m_camera) m_camera->Update(dt);
}

/*-------------------------------------------------------------*/
void Game::UpdateGameObjects(float dt)
{
    ASSERT(dt >= 0.0f);
    for (auto& obj : m_gameObjects)
        if (obj && obj->IsActive())
            obj->Update(dt);
}

/*-------------------------------------------------------------
  WASD + mouse look, zoom, and shooting input handling
--------------------------------------------------------------*/
void Game::HandleInput(float dt)
{
    ASSERT(dt >= 0.0f);
    if (!m_input || !m_camera) return;

    int dx = 0, dy = 0;
    if (m_input->GetMouseDelta(dx, dy))
    {
        constexpr float mouseSens = 0.005f;
        m_camera->Yaw(dx * mouseSens);
        m_camera->Pitch(-dy * mouseSens);
    }

    float moveSpeed = 10.0f * dt;
    if (m_input->IsKeyDown('W'))         m_camera->MoveForward(moveSpeed);
    if (m_input->IsKeyDown('S'))         m_camera->MoveForward(-moveSpeed);
    if (m_input->IsKeyDown('A'))         m_camera->MoveRight(-moveSpeed);
    if (m_input->IsKeyDown('D'))         m_camera->MoveRight(moveSpeed);
    if (m_input->IsKeyDown(VK_SPACE))    m_camera->MoveUp(moveSpeed);
    if (m_input->IsKeyDown(VK_LCONTROL)) m_camera->MoveUp(-moveSpeed);

    m_camera->SetZoom(m_input->IsMouseButtonDown(1));

    if (m_input->WasMouseButtonPressed(0) && m_projectilePool)
    {
        auto pos = m_camera->GetPosition();
        auto dir = m_camera->GetForward();
        m_projectilePool->FireProjectile(
            ProjectileType::BULLET, pos, dir, 50.0f);
    }
}

/*-------------------------------------------------------------
  Spawn placeholder objects
--------------------------------------------------------------*/
void Game::CreateTestObjects()
{
    std::wcerr << L"[DEBUG] CreateTestObjects() begin, will spawn placeholder objects\n";

    // Ground plane
    {
        auto ground = std::make_unique<PlaneObject>(20.0f, 20.0f);
        ASSERT(ground);
        HRESULT hr = ground->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        std::wcerr << L"[DEBUG] Ground mesh counts: Vertices="
            << ground->GetMesh()->GetVertexCount()
            << L", Indices=" << ground->GetMesh()->GetIndexCount() << std::endl;
        std::wcerr << (SUCCEEDED(hr)
            ? L"[DEBUG] Ground plane initialized\n"
            : L"[ERROR] Ground plane initialization FAILED\n");
        ground->SetPosition({ 0.0f, -1.0f, 0.0f });
        m_gameObjects.push_back(std::move(ground));
    }

    // Row of cubes
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>(1.0f);
        ASSERT(cube);
        HRESULT hr = cube->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        std::wcerr << L"[DEBUG] Cube " << i << L" mesh counts: Vertices="
            << cube->GetMesh()->GetVertexCount()
            << L", Indices=" << cube->GetMesh()->GetIndexCount() << std::endl;
        std::wcerr << (SUCCEEDED(hr)
            ? L"[DEBUG] Cube " + std::to_wstring(i) + L" initialized\n"
            : L"[ERROR] Cube " + std::to_wstring(i) + L" FAILED to initialize\n");
        cube->SetPosition({ i * 3.0f - 6.0f, 1.0f, 10.0f });
        m_gameObjects.push_back(std::move(cube));
    }

    // Single sphere
    {
        auto sphere = std::make_unique<SphereObject>(1.0f, 16, 16);
        ASSERT(sphere);
        HRESULT hr = sphere->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        std::wcerr << L"[DEBUG] Sphere mesh counts: Vertices="
            << sphere->GetMesh()->GetVertexCount()
            << L", Indices=" << sphere->GetMesh()->GetIndexCount() << std::endl;
        std::wcerr << (SUCCEEDED(hr)
            ? L"[DEBUG] Sphere initialized\n"
            : L"[ERROR] Sphere FAILED to initialize\n");
        sphere->SetPosition({ 5.0f, 0.0f, 0.0f });
        m_gameObjects.push_back(std::move(sphere));
    }

    std::wcerr << L"[DEBUG] Created total " << m_gameObjects.size() << L" game objects\n";
}