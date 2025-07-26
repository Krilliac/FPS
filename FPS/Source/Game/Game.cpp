#include "Game.h"
#include "..\Graphics\GraphicsEngine.h"
#include "..\Input\InputManager.h"
#include "..\Camera\FPSCamera.h"
#include "..\Graphics\Mesh.h"
#include "..\Graphics\Shader.h"
#include "GameObject.h"
#include "Player.h"
#include "..\Projectiles\ProjectilePool.h"

Game::Game()
    : m_graphics(nullptr)
    , m_input(nullptr)
    , m_isPaused(false)
{
}

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
    float aspectRatio = (float)m_graphics->GetWindowWidth() / (float)m_graphics->GetWindowHeight();
    m_camera->Initialize(aspectRatio);
    m_camera->SetPosition(XMFLOAT3(0.0f, 2.0f, -5.0f));

    // Initialize shader system
    m_shader = std::make_unique<Shader>();
    HRESULT hr = m_shader->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    if (FAILED(hr)) return hr;

    // Load shaders
    hr = m_shader->LoadVertexShader(L"Shaders\\HLSL\\BasicVS.hlsl");
    if (FAILED(hr)) return hr;
    
    hr = m_shader->LoadPixelShader(L"Shaders\\HLSL\\BasicPS.hlsl");
    if (FAILED(hr)) return hr;

    // Initialize player
    m_player = std::make_unique<Player>();
    hr = m_player->Initialize(m_graphics->GetDevice(), m_graphics->GetContext(), 
                             m_camera.get(), m_input);
    if (FAILED(hr)) return hr;

    // Initialize projectile pool
    m_projectilePool = std::make_unique<ProjectilePool>(100);
    hr = m_projectilePool->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    if (FAILED(hr)) return hr;

    // Create some test objects
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
    
    // Update player
    if (m_player)
    {
        m_player->Update(deltaTime);
    }

    // Update projectiles
    if (m_projectilePool)
    {
        m_projectilePool->Update(deltaTime);
    }
}

void Game::Render()
{
    if (!m_graphics || !m_camera || !m_shader) return;

    // Set up rendering pipeline
    m_shader->SetShaders();

    // Get matrices
    XMMATRIX view = m_camera->GetViewMatrix();
    XMMATRIX projection = m_camera->GetProjectionMatrix();

    // Render game objects
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject && gameObject->IsActive() && gameObject->IsVisible())
        {
            // Update constant buffer
            ConstantBuffer cb;
            cb.World = gameObject->GetWorldMatrix();
            cb.View = view;
            cb.Projection = projection;
            m_shader->UpdateConstantBuffer(cb);

            gameObject->Render(view, projection);
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
    if (m_camera)
    {
        m_camera->Update(deltaTime);
    }
}

void Game::UpdateGameObjects(float deltaTime)
{
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject && gameObject->IsActive())
        {
            gameObject->Update(deltaTime);
        }
    }
}

void Game::HandleInput(float deltaTime)
{
    if (!m_input || !m_camera) return;

    float moveSpeed = 10.0f * deltaTime;

    // Movement
    if (m_input->IsKeyDown('W'))
        m_camera->MoveForward(moveSpeed);
    if (m_input->IsKeyDown('S'))
        m_camera->MoveForward(-moveSpeed);
    if (m_input->IsKeyDown('A'))
        m_camera->MoveRight(-moveSpeed);
    if (m_input->IsKeyDown('D'))
        m_camera->MoveRight(moveSpeed);
    if (m_input->IsKeyDown(VK_SPACE))
        m_camera->MoveUp(moveSpeed);
    if (m_input->IsKeyDown(VK_LCONTROL))
        m_camera->MoveUp(-moveSpeed);

    // Mouse look
    int deltaX, deltaY;
    if (m_input->GetMouseDelta(deltaX, deltaY))
    {
        float sensitivity = 0.005f;
        m_camera->Yaw(deltaX * sensitivity);
        m_camera->Pitch(-deltaY * sensitivity);
    }

    // Shooting
    if (m_input->WasMouseButtonPressed(0) && m_projectilePool)
    {
        XMFLOAT3 position = m_camera->GetPosition();
        XMFLOAT3 direction = m_camera->GetForward();
        m_projectilePool->FireProjectile(position, direction, 50.0f);
    }
}

void Game::CreateTestObjects()
{
    // Create a ground plane
    auto ground = std::make_unique<PlaneObject>();
    ground->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
    ground->SetPosition(XMFLOAT3(0.0f, -1.0f, 0.0f));
    ground->SetScale(XMFLOAT3(20.0f, 1.0f, 20.0f));
    m_gameObjects.push_back(std::move(ground));

    // Create some test cubes
    for (int i = 0; i < 5; ++i)
    {
        auto cube = std::make_unique<CubeObject>();
        cube->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        cube->SetPosition(XMFLOAT3(i * 3.0f - 6.0f, 1.0f, 10.0f));
        m_gameObjects.push_back(std::move(cube));
    }
}
