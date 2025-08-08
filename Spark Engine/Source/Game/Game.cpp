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
#include "..\SceneManager\SceneManager.h"
#include "..\Utils\ConsoleProcessManager.h"

// Pull in globals defined in SparkEngine.cpp
extern std::unique_ptr<GraphicsEngine> g_graphics;
extern Console                         g_console;

// Helper macro for logging to external console
#define LOG_TO_CONSOLE(msg, type) Spark::ConsoleProcessManager::GetInstance().Log(msg, type)

/*-------------------------------------------------------------
  Ctor / Dtor
--------------------------------------------------------------*/
Game::Game() {
    LOG_TO_CONSOLE(L"Game constructor called.", L"INFO");
}
Game::~Game() {
    LOG_TO_CONSOLE(L"Game destructor called.", L"INFO");
    Shutdown();
}

/*-------------------------------------------------------------
  Initialise all game-side systems
--------------------------------------------------------------*/
HRESULT Game::Initialize(GraphicsEngine* graphics,
    InputManager* input)
{
    LOG_TO_CONSOLE(L"Game::Initialize called.", L"OPERATION");

    ASSERT(graphics != nullptr);
    ASSERT(input != nullptr);

    m_graphics = graphics;
    m_input = input;
    LOG_TO_CONSOLE(L"Graphics and InputManager assigned.", L"INFO");

    // SceneManager setup
    m_sceneManager = std::make_unique<SceneManager>(graphics, input);
    bool sceneLoaded = m_sceneManager->LoadScene(L"Assets/Scenes/level1.scene");
    LOG_TO_CONSOLE(L"SceneManager::LoadScene returned: " + std::wstring(sceneLoaded ? L"SUCCESS" : L"FAILURE"), L"INFO");

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
    LOG_TO_CONSOLE(L"Game initialization complete.", L"INFO");
    return S_OK;
}

/*-------------------------------------------------------------
  Tear-down
--------------------------------------------------------------*/
void Game::Shutdown()
{
    LOG_TO_CONSOLE(L"Game::Shutdown called.", L"OPERATION");

    m_gameObjects.clear();
    m_projectilePool.reset();
    m_player.reset();
    m_shader.reset();
    m_camera.reset();

    LOG_TO_CONSOLE(L"Game shutdown complete.", L"INFO");
}

/*-------------------------------------------------------------
  Per-frame update
--------------------------------------------------------------*/
void Game::Update(float dt)
{
    LOG_TO_CONSOLE(L"Game::Update called. dt=" + std::to_wstring(dt), L"OPERATION");

    if (m_isPaused) {
        LOG_TO_CONSOLE(L"Game is paused. Update skipped.", L"INFO");
        return;
    }

    HandleInput(dt);
    UpdateCamera(dt);
    UpdateGameObjects(dt);

    if (m_player)         m_player->Update(dt);
    if (m_projectilePool) m_projectilePool->Update(dt);

    LOG_TO_CONSOLE(L"Game update complete.", L"INFO");
}

/*-------------------------------------------------------------
  Per-frame render – called between BeginFrame / EndFrame
--------------------------------------------------------------*/
void Game::Render()
{
    LOG_TO_CONSOLE(L"Game::Render called.", L"OPERATION");

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
                LOG_TO_CONSOLE(L"Skipping SceneManager object in Render: active=" + std::to_wstring(obj ? obj->IsActive() : false) + L" visible=" + std::to_wstring(obj ? obj->IsVisible() : false), L"WARNING");
                continue;
            }
            ConstantBuffer cb{};
            cb.World = obj->GetWorldMatrix();
            cb.View = view;
            cb.Projection = proj;
            m_shader->UpdateConstantBuffer(cb);
            auto pos = obj->GetPosition();
            auto scale = obj->GetScale();
            auto rot = obj->GetRotation();
            std::wstring logMsg = L"[Render] Object ID=" + std::to_wstring(obj->GetID()) + L" Name=" + std::wstring(obj->GetName().begin(), obj->GetName().end()) +
                L" Pos=(" + std::to_wstring(pos.x) + L"," + std::to_wstring(pos.y) + L"," + std::to_wstring(pos.z) + L")" +
                L" Scale=(" + std::to_wstring(scale.x) + L"," + std::to_wstring(scale.y) + L"," + std::to_wstring(scale.z) + L")" +
                L" Rot=(" + std::to_wstring(rot.x) + L"," + std::to_wstring(rot.y) + L"," + std::to_wstring(rot.z) + L")";
            LOG_TO_CONSOLE(logMsg, L"INFO");
            obj->Render(view, proj);
            ++renderedCount;
        }
    }

    // Also render m_gameObjects (test objects)
    for (auto& obj : m_gameObjects) {
        if (!obj || !obj->IsActive() || !obj->IsVisible()) {
            ++skippedCount;
            LOG_TO_CONSOLE(L"Skipping game object in Render: active=" + std::to_wstring(obj ? obj->IsActive() : false) + L" visible=" + std::to_wstring(obj ? obj->IsVisible() : false), L"WARNING");
            continue;
        }
        ConstantBuffer cb{};
        cb.World = obj->GetWorldMatrix();
        cb.View = view;
        cb.Projection = proj;
        m_shader->UpdateConstantBuffer(cb);
        auto pos = obj->GetPosition();
        auto scale = obj->GetScale();
        auto rot = obj->GetRotation();
        std::wstring logMsg = L"[Render] GameObject ID=" + std::to_wstring(obj->GetID()) + L" Name=" + std::wstring(obj->GetName().begin(), obj->GetName().end()) +
            L" Pos=(" + std::to_wstring(pos.x) + L"," + std::to_wstring(pos.y) + L"," + std::to_wstring(pos.z) + L")" +
            L" Scale=(" + std::to_wstring(scale.x) + L"," + std::to_wstring(scale.y) + L"," + std::to_wstring(scale.z) + L")" +
            L" Rot=(" + std::to_wstring(rot.x) + L"," + std::to_wstring(rot.y) + L"," + std::to_wstring(rot.z) + L")";
        LOG_TO_CONSOLE(logMsg, L"INFO");
        obj->Render(view, proj);
        ++renderedCount;
    }

    static int frameNum = 0;
    if (++frameNum % 60 == 0) {
        LOG_TO_CONSOLE(L"[DEBUG] Rendered " + std::to_wstring(renderedCount) + L" objects, skipped " + std::to_wstring(skippedCount) + L" this frame", L"INFO");
    }

    if (m_player)         m_player->Render(view, proj);
    if (m_projectilePool) m_projectilePool->Render(view, proj);

    if (g_console.IsVisible())
        g_console.Render(m_graphics->GetContext());

    LOG_TO_CONSOLE(L"Game::Render complete.", L"INFO");
}

/*-------------------------------------------------------------*/
void Game::UpdateCamera(float dt)
{
    LOG_TO_CONSOLE(L"Game::UpdateCamera called. dt=" + std::to_wstring(dt), L"OPERATION");

    ASSERT(dt >= 0.0f);
    if (m_camera) m_camera->Update(dt);
}

/*-------------------------------------------------------------*/
void Game::UpdateGameObjects(float dt)
{
    LOG_TO_CONSOLE(L"Game::UpdateGameObjects called. dt=" + std::to_wstring(dt), L"OPERATION");

    ASSERT(dt >= 0.0f);
    for (auto& obj : m_gameObjects)
        if (obj && obj->IsActive()) obj->Update(dt);
}

/*-------------------------------------------------------------
  WASD + mouse look, zoom, and shooting input handling
--------------------------------------------------------------*/
void Game::HandleInput(float dt)
{
    LOG_TO_CONSOLE(L"Game::HandleInput called. dt=" + std::to_wstring(dt), L"OPERATION");

    ASSERT(dt >= 0.0f);
    if (!m_input || !m_camera) {
        LOG_TO_CONSOLE(L"Input or Camera is null in HandleInput.", L"ERROR");
        return;
    }

    int dx = 0, dy = 0;
    if (m_input->GetMouseDelta(dx, dy)) {
        constexpr float mouseSens = 0.005f;
        m_camera->Yaw(dx * mouseSens);
        m_camera->Pitch(-dy * mouseSens);
        LOG_TO_CONSOLE(L"Mouse delta applied: dx=" + std::to_wstring(dx) + L" dy=" + std::to_wstring(dy), L"INFO");
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
        LOG_TO_CONSOLE(L"Fired projectile from pos=(" + std::to_wstring(pos.x) + L"," + std::to_wstring(pos.y) + L"," + std::to_wstring(pos.z) + L") dir=(" + std::to_wstring(dir.x) + L"," + std::to_wstring(dir.y) + L"," + std::to_wstring(dir.z) + L")", L"INFO");
    }
}

/*-------------------------------------------------------------
  Spawn placeholder objects
--------------------------------------------------------------*/
void Game::CreateTestObjects()
{
    LOG_TO_CONSOLE(L"CreateTestObjects() begin, will spawn placeholder objects", L"OPERATION");

    // Ground plane
    {
        auto ground = std::make_unique<PlaneObject>(20.0f, 20.0f);
        ASSERT(ground);
        HRESULT hr = ground->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        LOG_TO_CONSOLE(L"Ground mesh counts: Vertices=" + std::to_wstring(ground->GetMesh()->GetVertexCount()) + L", Indices=" + std::to_wstring(ground->GetMesh()->GetIndexCount()), L"DEBUG");
        LOG_TO_CONSOLE(SUCCEEDED(hr) ? L"Ground plane initialized" : L"Ground plane initialization FAILED", SUCCEEDED(hr) ? L"DEBUG" : L"ERROR");
        ground->SetPosition({ 0.0f, -1.0f, 0.0f });
        m_gameObjects.push_back(std::move(ground));
    }

    // Row of cubes
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>(1.0f);
        ASSERT(cube);
        HRESULT hr = cube->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        LOG_TO_CONSOLE(L"Cube " + std::to_wstring(i) + L" mesh counts: Vertices=" + std::to_wstring(cube->GetMesh()->GetVertexCount()) + L", Indices=" + std::to_wstring(cube->GetMesh()->GetIndexCount()), L"DEBUG");
        LOG_TO_CONSOLE(SUCCEEDED(hr) ? L"Cube " + std::to_wstring(i) + L" initialized" : L"Cube " + std::to_wstring(i) + L" FAILED to initialize", SUCCEEDED(hr) ? L"DEBUG" : L"ERROR");
        cube->SetPosition({ i * 3.0f - 6.0f, 1.0f, 10.0f });
        m_gameObjects.push_back(std::move(cube));
    }

    // Single sphere
    {
        auto sphere = std::make_unique<SphereObject>(1.0f, 16, 16);
        ASSERT(sphere);
        HRESULT hr = sphere->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        LOG_TO_CONSOLE(L"Sphere mesh counts: Vertices=" + std::to_wstring(sphere->GetMesh()->GetVertexCount()) + L", Indices=" + std::to_wstring(sphere->GetMesh()->GetIndexCount()), L"DEBUG");
        LOG_TO_CONSOLE(SUCCEEDED(hr) ? L"Sphere initialized" : L"Sphere FAILED to initialize", SUCCEEDED(hr) ? L"DEBUG" : L"ERROR");
        sphere->SetPosition({ 5.0f, 0.0f, 0.0f });
        m_gameObjects.push_back(std::move(sphere));
    }

    LOG_TO_CONSOLE(L"Created total " + std::to_wstring(m_gameObjects.size()) + L" game objects", L"INFO");
}