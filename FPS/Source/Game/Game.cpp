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
#include "..\Projectiles\ProjectilePool.h"

Game::Game() = default;

Game::~Game()
{
    Shutdown();
}

HRESULT Game::Initialize(GraphicsEngine* graphics, InputManager* input)
{
    m_graphics = graphics;
    m_input = input;

    // Initialize camera
    m_camera = std::make_unique<FPSCamera>();
    float aspect = float(m_graphics->GetWindowWidth()) / m_graphics->GetWindowHeight();
    m_camera->Initialize(aspect);
    m_camera->SetPosition({ 0.0f, 2.0f, -5.0f });

    // Initialize shader system
    m_shader = std::make_unique<Shader>();
    HRESULT hr = m_shader->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    if (FAILED(hr)) return hr;

    hr = m_shader->LoadVertexShader(L"Shaders\\HLSL\\BasicVS.hlsl");
    if (FAILED(hr)) return hr;
    hr = m_shader->LoadPixelShader(L"Shaders\\HLSL\\BasicPS.hlsl");
    if (FAILED(hr)) return hr;

    // Initialize player
    m_player = std::make_unique<Player>();
    hr = m_player->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext(),
        m_camera.get(),
        m_input
    );
    if (FAILED(hr)) return hr;

    // Initialize projectile pool
    m_projectilePool = std::make_unique<ProjectilePool>(100);
    hr = m_projectilePool->Initialize(
        m_graphics->GetDevice(),
        m_graphics->GetContext()
    );
    if (FAILED(hr)) return hr;

    // Populate world
    CreateTestObjects();
    return S_OK;
}

void Game::Shutdown()
{
    m_gameObjects.clear();
    m_projectilePool.reset();
    m_player.reset();
    m_shader.reset();
    m_camera.reset();
}

void Game::Update(float deltaTime)
{
    if (m_isPaused) return;

    HandleInput(deltaTime);
    UpdateCamera(deltaTime);
    UpdateGameObjects(deltaTime);

    if (m_player)            m_player->Update(deltaTime);
    if (m_projectilePool)    m_projectilePool->Update(deltaTime);
}

void Game::Render()
{
    if (!m_graphics || !m_camera || !m_shader) return;

    m_shader->SetShaders();
    XMMATRIX view = m_camera->GetViewMatrix();
    XMMATRIX projection = m_camera->GetProjectionMatrix();

    // Render scene objects
    ConstantBuffer cb;
    for (auto& obj : m_gameObjects)
    {
        if (obj && obj->IsActive() && obj->IsVisible())
        {
            cb.World = obj->GetWorldMatrix();
            cb.View = view;
            cb.Projection = projection;
            m_shader->UpdateConstantBuffer(cb);
            obj->Render(view, projection);
        }
    }

    // Render projectiles
    if (m_projectilePool)
    {
        m_projectilePool->Render(view, projection);
    }
}

void Game::UpdateCamera(float deltaTime)
{
    if (m_camera) m_camera->Update(deltaTime);
}

void Game::UpdateGameObjects(float deltaTime)
{
    for (auto& obj : m_gameObjects)
    {
        if (obj && obj->IsActive())
            obj->Update(deltaTime);
    }
}

void Game::HandleInput(float deltaTime)
{
    if (!m_input || !m_camera) return;

    float moveSpeed = 10.0f * deltaTime;
    if (m_input->IsKeyDown('W')) m_camera->MoveForward(moveSpeed);
    if (m_input->IsKeyDown('S')) m_camera->MoveForward(-moveSpeed);
    if (m_input->IsKeyDown('A')) m_camera->MoveRight(-moveSpeed);
    if (m_input->IsKeyDown('D')) m_camera->MoveRight(moveSpeed);
    if (m_input->IsKeyDown(VK_SPACE))    m_camera->MoveUp(moveSpeed);
    if (m_input->IsKeyDown(VK_LCONTROL)) m_camera->MoveUp(-moveSpeed);

    int dx, dy;
    if (m_input->GetMouseDelta(dx, dy))
    {
        float sens = 0.005f;
        m_camera->Yaw(dx * sens);
        m_camera->Pitch(-dy * sens);
    }

    if (m_input->WasMouseButtonPressed(0) && m_projectilePool)
    {
        XMFLOAT3 pos = m_camera->GetPosition();
        XMFLOAT3 dir = m_camera->GetForward();
        m_projectilePool->FireProjectile(
            ProjectileType::BULLET,
            pos, dir,
            50.0f
        );
    }
}

void Game::CreateTestObjects()
{
    // Ground plane placeholder or asset
    auto ground = std::make_unique<PlaneObject>(20.0f, 20.0f);
    ground->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    ground->SetPosition({ 0, -1, 0 });
    m_gameObjects.push_back(std::move(ground));

    // Array of cubes
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>(1.0f);
        cube->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        cube->SetPosition({ i * 3.0f - 6.0f, 1.0f, 10.0f });
        m_gameObjects.push_back(std::move(cube));
    }

    // Single sphere
    auto sphere = std::make_unique<SphereObject>(1.0f, 16, 16);
    sphere->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    sphere->SetPosition({ 5.0f, 0.0f, 0.0f });
    m_gameObjects.push_back(std::move(sphere));
}