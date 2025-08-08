/**
 * @file Game.h
 * @brief Main game class managing the core game loop and scene systems
 * @author Spark Engine Team
 * @date 2025
 * 
 * The Game class serves as the central coordinator for all game systems,
 * managing the main game loop, scene updates, rendering operations, and
 * coordination between different engine subsystems like graphics, input,
 * camera, and game objects.
 */

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
#include "SceneManager.h"

/**
 * @brief Main game controller class managing the game loop and scene
 * 
 * The Game class is responsible for coordinating all game systems and managing
 * the main game loop. It handles initialization of game objects, processing
 * user input, updating game state, and rendering the scene. This class serves
 * as the bridge between the engine systems and the actual game content.
 * 
 * @note The Game class does not own the graphics or input systems, but holds references
 * @warning Ensure Initialize() is called before any Update() or Render() operations
 */
class Game
{
public:
    /**
     * @brief Default constructor
     * 
     * Initializes member variables to safe default values. The actual game
     * systems are set up in the Initialize() method.
     */
    Game();

    /**
     * @brief Destructor
     * 
     * Automatically calls Shutdown() to ensure proper cleanup of all game systems.
     */
    ~Game();

    /**
     * @brief Initialize the game systems and scene
     * 
     * Sets up all game systems including camera, shaders, player, projectile pool,
     * and creates initial test objects in the scene. Must be called after the
     * graphics and input systems are initialized.
     * 
     * @param graphics Pointer to the initialized graphics engine
     * @param input Pointer to the initialized input manager
     * @return HRESULT indicating success or failure of game initialization
     * @note The graphics and input pointers are stored but not owned by Game
     */
    HRESULT Initialize(GraphicsEngine* graphics, InputManager* input);

    /**
     * @brief Clean up all game systems and objects
     * 
     * Shuts down all game subsystems and clears the scene. Safe to call
     * multiple times.
     */
    void    Shutdown();

    /**
     * @brief Update the game state for the current frame
     * 
     * Processes input, updates camera, updates all game objects, and handles
     * game logic. Called once per frame during the main game loop.
     * 
     * @param deltaTime Time elapsed since the last frame in seconds
     * @note If the game is paused, this method returns immediately
     */
    void Update(float deltaTime);

    /**
     * @brief Render the current frame
     * 
     * Issues rendering commands for all visible game objects and manages
     * the rendering pipeline for the current frame. Should be called after
     * Update() during the main game loop.
     */
    void Render();

    /**
     * @brief Pause the game simulation
     * 
     * Stops game updates while maintaining rendering. Useful for pause menus
     * or when the game window loses focus.
     */
    void Pause() { m_isPaused = true; }

    /**
     * @brief Resume the game simulation
     * 
     * Resumes game updates from a paused state.
     */
    void Resume() { m_isPaused = false; }

    /**
     * @brief Check if the game is currently paused
     * @return true if game is paused, false if running
     */
    bool IsPaused() const { return m_isPaused; }

private:
    /**
     * @brief Update the camera based on input and game state
     * @param dt Delta time for frame-rate independent movement
     */
    void UpdateCamera(float dt);

    /**
     * @brief Update all game objects in the scene
     * @param dt Delta time for frame-rate independent updates
     */
    void UpdateGameObjects(float dt);

    /**
     * @brief Process input and translate to game actions
     * @param dt Delta time for frame-rate independent input handling
     */
    void HandleInput(float dt);

    /**
     * @brief Create initial test objects for the scene
     * 
     * Populates the scene with basic objects for testing and demonstration
     * purposes. Called during initialization.
     */
    void CreateTestObjects();

    // Engine-side pointers (not owned)
    GraphicsEngine* m_graphics{ nullptr }; ///< Reference to graphics engine
    InputManager* m_input{ nullptr };      ///< Reference to input manager

    // Sub-systems owned by Game
    std::unique_ptr<SparkEngineCamera> m_camera;        ///< First-person camera system
    std::unique_ptr<Shader>            m_shader;        ///< Shader management
    std::unique_ptr<Player>            m_player;        ///< Player controller
    std::unique_ptr<ProjectilePool>    m_projectilePool; ///< Projectile object pool
    std::unique_ptr<SceneManager>      m_sceneManager;    ///< Scene management

    // Scene objects
    std::vector<std::unique_ptr<GameObject>> m_gameObjects; ///< All game objects in the scene

    bool m_isPaused{ false }; ///< Current pause state of the game
};