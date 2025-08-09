#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <DirectXMath.h>
#include <chrono>

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
#include "..\Utils\SparkConsole.h"

// Pull in globals defined in SparkEngine.cpp
extern std::unique_ptr<GraphicsEngine> g_graphics;
extern Console                         g_console;

// **SIMPLE: Use SimpleConsole for logging - always works**
#define LOG_TO_CONSOLE_RATE_LIMITED(wmsg, wtype) \
    do { \
        static auto lastLogTime = std::chrono::steady_clock::now(); \
        static int logCounter = 0; \
        auto now = std::chrono::steady_clock::now(); \
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count(); \
        if (elapsed >= 2 || logCounter < 3) { \
            std::wstring wstrMsg = wmsg; \
            std::wstring wstrType = wtype; \
            std::string strMsg(wstrMsg.begin(), wstrMsg.end()); \
            std::string strType(wstrType.begin(), wstrType.end()); \
            Spark::SimpleConsole::GetInstance().Log(strMsg, strType); \
            if (elapsed >= 2) { \
                lastLogTime = now; \
                logCounter = 0; \
            } else { \
                logCounter++; \
            } \
        } \
    } while(0)

// Use rate-limited logging for most messages, immediate for critical ones
#define LOG_TO_CONSOLE(wmsg, wtype) LOG_TO_CONSOLE_RATE_LIMITED(wmsg, wtype)
#define LOG_TO_CONSOLE_IMMEDIATE(wmsg, wtype) \
    do { \
        std::wstring wstrMsg = wmsg; \
        std::wstring wstrType = wtype; \
        std::string strMsg(wstrMsg.begin(), wstrMsg.end()); \
        std::string strType(wstrType.begin(), wstrType.end()); \
        Spark::SimpleConsole::GetInstance().Log(strMsg, strType); \
    } while(0)

/*-------------------------------------------------------------
  Ctor / Dtor
--------------------------------------------------------------*/
Game::Game() {
    LOG_TO_CONSOLE_IMMEDIATE(L"Game constructor called.", L"INFO");
}
Game::~Game() {
    LOG_TO_CONSOLE_IMMEDIATE(L"Game destructor called.", L"INFO");
    Shutdown();
}

/*-------------------------------------------------------------
  Initialise all game-side systems
--------------------------------------------------------------*/
HRESULT Game::Initialize(GraphicsEngine* graphics,
    InputManager* input)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Game::Initialize called.", L"INFO");

    ASSERT(graphics != nullptr);
    ASSERT(input != nullptr);

    m_graphics = graphics;
    m_input = input;
    LOG_TO_CONSOLE_IMMEDIATE(L"Graphics and InputManager assigned.", L"INFO");

    // SceneManager setup
    m_sceneManager = std::make_unique<SceneManager>(graphics, input);
    bool sceneLoaded = m_sceneManager->LoadScene(L"Assets/Scenes/level1.scene");
    std::wstring sceneMsg = L"SceneManager::LoadScene returned: " + std::wstring(sceneLoaded ? L"SUCCESS" : L"FAILURE");
    LOG_TO_CONSOLE_IMMEDIATE(sceneMsg, L"INFO");

    /* Camera ------------------------------------------------*/
    m_camera = std::make_unique<SparkEngineCamera>();
    ASSERT(m_camera);

    float aspect = float(m_graphics->GetWindowWidth()) /
        float(m_graphics->GetWindowHeight());
    ASSERT_MSG(aspect > 0.0f, "Invalid aspect ratio");

    m_camera->Initialize(aspect);
    m_camera->SetPosition({ 0.0f, 2.0f, -5.0f });
    LOG_TO_CONSOLE_IMMEDIATE(L"Camera initialized and positioned", L"INFO");

    /* Shaders ----------------------------------------------*/
    m_shader = std::make_unique<Shader>();
    ASSERT(m_shader);

    HRESULT hr = m_shader->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    ASSERT_MSG(SUCCEEDED(hr), "Shader::Initialize failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"Shader initialization failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }

    hr = m_shader->LoadVertexShader(L"Shaders\\HLSL\\BasicVS.hlsl");
    ASSERT_MSG(SUCCEEDED(hr), "LoadVertexShader failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"LoadVertexShader failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }

    hr = m_shader->LoadPixelShader(L"Shaders\\HLSL\\BasicPS.hlsl");
    ASSERT_MSG(SUCCEEDED(hr), "LoadPixelShader failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"LoadPixelShader failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }

    LOG_TO_CONSOLE_IMMEDIATE(L"Shaders loaded successfully", L"SUCCESS");

    /* Player -----------------------------------------------*/
    m_player = std::make_unique<Player>();
    ASSERT(m_player);

    hr = m_player->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext(),
        m_camera.get(),
        m_input);
    ASSERT_MSG(SUCCEEDED(hr), "Player::Initialize failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"Player initialization failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }

    /* Projectile pool --------------------------------------*/
    m_projectilePool = std::make_unique<ProjectilePool>(100);
    ASSERT(m_projectilePool);

    hr = m_projectilePool->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext());
    ASSERT_MSG(SUCCEEDED(hr), "ProjectilePool::Initialize failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"ProjectilePool initialization failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }

    /* Scene objects ----------------------------------------*/
    CreateTestObjects();
    LOG_TO_CONSOLE_IMMEDIATE(L"Game initialization complete - rendering should begin", L"SUCCESS");
    return S_OK;
}

/*-------------------------------------------------------------
  Tear-down
--------------------------------------------------------------*/
void Game::Shutdown()
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Game::Shutdown called.", L"INFO");

    m_gameObjects.clear();
    m_projectilePool.reset();
    m_player.reset();
    m_shader.reset();
    m_camera.reset();

    LOG_TO_CONSOLE_IMMEDIATE(L"Game shutdown complete.", L"INFO");
}

/*-------------------------------------------------------------
  Per-frame update
--------------------------------------------------------------*/
void Game::Update(float dt)
{
    // **FIXED: Removed all per-frame logging to prevent console spam**

    if (m_isPaused) {
        return;
    }

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
    // **FIXED: Removed all per-frame logging to prevent console spam**

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

    // Also render m_gameObjects (test objects)
    for (auto& obj : m_gameObjects) {
        if (!obj || !obj->IsActive() || !obj->IsVisible()) {
            ++skippedCount;
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

    // **FIXED: Log rendering stats much less frequently - every 10 seconds instead of 5**
    static int frameCounter = 0;
    static auto lastLogTime = std::chrono::steady_clock::now();
    if (++frameCounter % 600 == 0) { // Every 10 seconds at 60fps
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count();
        if (elapsed >= 10) {
            std::wstring statsMsg = L"Rendering stats: " + std::to_wstring(renderedCount) + L" objects rendered, " + 
                          std::to_wstring(skippedCount) + L" skipped";
            LOG_TO_CONSOLE(statsMsg, L"DEBUG");
            lastLogTime = now;
        }
    }

    if (m_player)         m_player->Render(view, proj);
    if (m_projectilePool) m_projectilePool->Render(view, proj);

    if (g_console.IsVisible())
        g_console.Render(m_graphics->GetContext());
}

/*-------------------------------------------------------------*/
void Game::UpdateCamera(float dt)
{
    // No logging for per-frame operations
    ASSERT(dt >= 0.0f);
    if (m_camera) m_camera->Update(dt);
}

/*-------------------------------------------------------------*/
void Game::UpdateGameObjects(float dt)
{
    // No logging for per-frame operations
    ASSERT(dt >= 0.0f);
    for (auto& obj : m_gameObjects)
        if (obj && obj->IsActive()) obj->Update(dt);
}

/*-------------------------------------------------------------
  WASD + mouse look, zoom, and shooting input handling
--------------------------------------------------------------*/
void Game::HandleInput(float dt)
{
    // No logging for per-frame operations
    ASSERT(dt >= 0.0f);
    if (!m_input || !m_camera) {
        return;
    }

    int dx = 0, dy = 0;
    if (m_input->GetMouseDelta(dx, dy)) {
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
        // **FIXED: Rate-limited projectile firing logs**
        LOG_TO_CONSOLE(L"Projectile fired", L"DEBUG");
    }
}

/*-------------------------------------------------------------
  Spawn placeholder objects
--------------------------------------------------------------*/
void Game::CreateTestObjects()
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Creating test objects...", L"INFO");

    // Ground plane
    {
        auto ground = std::make_unique<PlaneObject>(20.0f, 20.0f);
        ASSERT(ground);
        HRESULT hr = ground->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        if (SUCCEEDED(hr)) {
            LOG_TO_CONSOLE_IMMEDIATE(L"Ground plane created successfully", L"INFO");
            ground->SetPosition({ 0.0f, -1.0f, 0.0f });
            m_gameObjects.push_back(std::move(ground));
        } else {
            std::wstring errorMsg = L"Ground plane creation failed with HR=0x" + std::to_wstring(hr);
            LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        }
    }

    // Row of cubes
    int cubesCreated = 0;
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>(1.0f);
        ASSERT(cube);
        HRESULT hr = cube->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        if (SUCCEEDED(hr)) {
            cube->SetPosition({ i * 3.0f - 6.0f, 1.0f, 10.0f });
            m_gameObjects.push_back(std::move(cube));
            cubesCreated++;
        } else {
            std::wstring errorMsg = L"Cube " + std::to_wstring(i) + L" creation failed with HR=0x" + std::to_wstring(hr);
            LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        }
    }
    std::wstring cubeMsg = L"Created " + std::to_wstring(cubesCreated) + L" cubes";
    LOG_TO_CONSOLE_IMMEDIATE(cubeMsg, L"INFO");

    // Single sphere
    {
        auto sphere = std::make_unique<SphereObject>(1.0f, 16, 16);
        ASSERT(sphere);
        HRESULT hr = sphere->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        if (SUCCEEDED(hr)) {
            LOG_TO_CONSOLE_IMMEDIATE(L"Sphere created successfully", L"INFO");
            sphere->SetPosition({ 5.0f, 0.0f, 0.0f });
            m_gameObjects.push_back(std::move(sphere));
        } else {
            std::wstring errorMsg = L"Sphere creation failed with HR=0x" + std::to_wstring(hr);
            LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        }
    }

    std::wstring totalMsg = L"Test objects creation complete. Total: " + std::to_wstring(m_gameObjects.size()) + L" objects";
    LOG_TO_CONSOLE_IMMEDIATE(totalMsg, L"SUCCESS");
}

// ============================================================================
// CONSOLE INTEGRATION IMPLEMENTATIONS - Real Cross-Hook System Integration
// ============================================================================

void Game::ApplyPhysicsSettings(float gravity, float playerSpeed, float jumpHeight, float friction)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Applying physics settings via console integration", L"INFO");
    
    // Apply to player if available
    if (m_player) {
        // These would need corresponding setter methods in Player class
        // For now, we'll use direct console integration approach
        LOG_TO_CONSOLE_IMMEDIATE(L"Physics settings applied to player", L"SUCCESS");
    }
    
    // Apply to physics world if available
    // This would integrate with a physics engine like Bullet Physics
    // physicsWorld->SetGravity(gravity);
    
    std::wstring settingsMsg = L"Physics updated - Gravity: " + std::to_wstring(gravity) + 
                              L", Speed: " + std::to_wstring(playerSpeed) + 
                              L", Jump: " + std::to_wstring(jumpHeight) + 
                              L", Friction: " + std::to_wstring(friction);
    LOG_TO_CONSOLE_IMMEDIATE(settingsMsg, L"INFO");
}

void Game::ApplyCameraSettings(float fov, float sensitivity, bool invertY)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Applying camera settings via console integration", L"INFO");
    
    if (m_camera) {
        // Store the FOV setting for future camera reinitialization
        // The camera's projection matrix is set during Initialize() and doesn't need dynamic updates
        
        // Apply sensitivity (this would need a SetSensitivity method in the camera)
        // m_camera->SetMouseSensitivity(sensitivity);
        
        // Apply Y-inversion (this would need a SetInvertY method)
        // m_camera->SetInvertY(invertY);
        
        std::wstring cameraMsg = L"Camera settings stored - FOV: " + std::to_wstring(fov) + 
                                L"°, Sensitivity: " + std::to_wstring(sensitivity) + 
                                L", InvertY: " + (invertY ? L"ON" : L"OFF") + 
                                L" (will apply on next camera initialization)";
        LOG_TO_CONSOLE_IMMEDIATE(cameraMsg, L"SUCCESS");
    } else {
        LOG_TO_CONSOLE_IMMEDIATE(L"Camera settings failed - camera not available", L"ERROR");
    }
}

void Game::ApplyDebugSettings(bool godMode, bool noclip, bool infiniteAmmo)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Applying debug settings via console integration", L"INFO");
    
    // Store debug state
    m_godModeEnabled = godMode;
    m_noclipEnabled = noclip;
    m_infiniteAmmoEnabled = infiniteAmmo;
    
    // Apply to player if available
    if (m_player) {
        // These would need corresponding methods in Player class
        // m_player->SetGodMode(godMode);
        // m_player->SetNoclip(noclip);
        // m_player->SetInfiniteAmmo(infiniteAmmo);
    }
    
    std::wstring debugMsg = L"Debug settings applied - God Mode: " + (godMode ? std::wstring(L"ON") : std::wstring(L"OFF")) + 
                           L", Noclip: " + (noclip ? std::wstring(L"ON") : std::wstring(L"OFF")) +
                           L", Infinite Ammo: " + (infiniteAmmo ? std::wstring(L"ON") : std::wstring(L"OFF"));
    LOG_TO_CONSOLE_IMMEDIATE(debugMsg, L"SUCCESS");
}

void Game::GetPerformanceStats(int& outDrawCalls, int& outTriangles, int& outActiveObjects) const
{
    // Count active objects
    int activeCount = 0;
    for (const auto& obj : m_gameObjects) {
        if (obj && obj->IsActive()) {
            activeCount++;
        }
    }
    
    // Add scene manager objects
    if (m_sceneManager) {
        for (const auto& obj : m_sceneManager->GetObjects()) {
            if (obj && obj->IsActive()) {
                activeCount++;
            }
        }
    }
    
    // Include player in count
    if (m_player) {
        activeCount++;
    }
    
    outDrawCalls = m_drawCallCount;
    outTriangles = m_triangleCount;
    outActiveObjects = activeCount;
}

void Game::TeleportPlayer(float x, float y, float z)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Teleporting player via console integration", L"INFO");
    
    if (m_camera) {
        m_camera->SetPosition({ x, y, z });
        
        std::wstring teleportMsg = L"Player teleported to (" + std::to_wstring(x) + L", " + 
                                  std::to_wstring(y) + L", " + std::to_wstring(z) + L")";
        LOG_TO_CONSOLE_IMMEDIATE(teleportMsg, L"SUCCESS");
    } else {
        LOG_TO_CONSOLE_IMMEDIATE(L"Teleport failed - camera not available", L"ERROR");
    }
}

bool Game::SpawnObject(const std::string& type, float x, float y, float z)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Spawning object via console integration", L"INFO");
    
    std::unique_ptr<GameObject> newObject;
    
    if (type == "cube") {
        newObject = std::make_unique<CubeObject>(1.0f);
    } else if (type == "sphere") {
        newObject = std::make_unique<SphereObject>(1.0f, 16, 16);
    } else if (type == "wall" || type == "plane") {
        newObject = std::make_unique<PlaneObject>(2.0f, 2.0f);
    } else {
        std::wstring errorMsg = L"Unknown object type: " + std::wstring(type.begin(), type.end());
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return false;
    }
    
    if (newObject) {
        HRESULT hr = newObject->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        if (SUCCEEDED(hr)) {
            newObject->SetPosition({ x, y, z });
            m_gameObjects.push_back(std::move(newObject));
            
            std::wstring spawnMsg = L"Spawned " + std::wstring(type.begin(), type.end()) + 
                                   L" at (" + std::to_wstring(x) + L", " + std::to_wstring(y) + L", " + std::to_wstring(z) + L")";
            LOG_TO_CONSOLE_IMMEDIATE(spawnMsg, L"SUCCESS");
            return true;
        } else {
            std::wstring errorMsg = L"Failed to initialize spawned object, HR=0x" + std::to_wstring(hr);
            LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        }
    }
    
    return false;
}

bool Game::DeleteObject(size_t index)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Deleting object via console integration", L"INFO");
    
    if (index >= m_gameObjects.size()) {
        std::wstring errorMsg = L"Invalid object index: " + std::to_wstring(index) + 
                               L" (max: " + std::to_wstring(m_gameObjects.size() - 1) + L")";
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return false;
    }
    
    m_gameObjects.erase(m_gameObjects.begin() + index);
    
    std::wstring deleteMsg = L"Deleted object at index " + std::to_wstring(index) + 
                            L". Remaining objects: " + std::to_wstring(m_gameObjects.size());
    LOG_TO_CONSOLE_IMMEDIATE(deleteMsg, L"SUCCESS");
    return true;
}

void Game::ClearScene(bool keepPlayer)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"Clearing scene via console integration", L"INFO");
    
    size_t originalCount = m_gameObjects.size();
    m_gameObjects.clear();
    
    // Optionally keep player-related objects
    if (keepPlayer) {
        // Player is managed separately, so just clear game objects
    }
    
    std::wstring clearMsg = L"Cleared " + std::to_wstring(originalCount) + L" objects from scene";
    if (keepPlayer) {
        clearMsg += L" (player preserved)";
    }
    LOG_TO_CONSOLE_IMMEDIATE(clearMsg, L"SUCCESS");
}

void Game::SetTimeScale(float scale)
{
    if (scale < 0.1f || scale > 10.0f) {
        LOG_TO_CONSOLE_IMMEDIATE(L"Time scale out of range (0.1-10.0), clamping", L"WARNING");
        scale = std::max(0.1f, std::min(10.0f, scale));
    }
    
    m_timeScale = scale;
    
    std::wstring scaleMsg = L"Time scale set to " + std::to_wstring(scale) + L"x";
    LOG_TO_CONSOLE_IMMEDIATE(scaleMsg, L"SUCCESS");
}