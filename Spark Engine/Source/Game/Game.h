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
#include "SceneManager/SceneManager.h"

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

    // ============================================================================
    // CONSOLE INTEGRATION ACCESSORS - Real Cross-Hook System Integration
    // ============================================================================
    
    /**
     * @brief Get the player instance for console integration
     * @return Pointer to the player object, nullptr if not initialized
     */
    Player* GetPlayer() const { return m_player.get(); }
    
    /**
     * @brief Get the camera instance for console integration
     * @return Pointer to the camera object, nullptr if not initialized
     */
    SparkEngineCamera* GetCamera() const { return m_camera.get(); }
    
    /**
     * @brief Get the projectile pool for console integration
     * @return Pointer to the projectile pool, nullptr if not initialized
     */
    ProjectilePool* GetProjectilePool() const { return m_projectilePool.get(); }
    
    /**
     * @brief Get the scene manager for console integration
     * @return Pointer to the scene manager, nullptr if not initialized
     */
    SceneManager* GetSceneManager() const { return m_sceneManager.get(); }
    
    /**
     * @brief Get all game objects in the scene
     * @return Const reference to the game objects vector
     */
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return m_gameObjects; }
    
    /**
     * @brief Get mutable access to game objects for console commands
     * @return Reference to the game objects vector
     */
    std::vector<std::unique_ptr<GameObject>>& GetGameObjectsMutable() { return m_gameObjects; }
    
    /**
     * @brief Apply console-driven physics settings in real-time
     * @param gravity New gravity value
     * @param playerSpeed New player movement speed
     * @param jumpHeight New player jump height
     * @param friction New physics friction coefficient
     */
    void ApplyPhysicsSettings(float gravity, float playerSpeed, float jumpHeight, float friction);
    
    /**
     * @brief Apply console-driven camera settings in real-time
     * @param fov New field of view in degrees
     * @param sensitivity New mouse sensitivity
     * @param invertY Whether to invert Y-axis
     */
    void ApplyCameraSettings(float fov, float sensitivity, bool invertY);
    
    /**
     * @brief Apply console-driven debug settings
     * @param godMode Enable/disable god mode
     * @param noclip Enable/disable collision
     * @param infiniteAmmo Enable/disable infinite ammunition
     */
    void ApplyDebugSettings(bool godMode, bool noclip, bool infiniteAmmo);
    
    /**
     * @brief Get current performance statistics for console display
     * @param outDrawCalls Number of draw calls this frame
     * @param outTriangles Number of triangles rendered
     * @param outActiveObjects Number of active objects in scene
     */
    void GetPerformanceStats(int& outDrawCalls, int& outTriangles, int& outActiveObjects) const;
    
    /**
     * @brief Teleport player to specific coordinates via console
     * @param x Target X coordinate
     * @param y Target Y coordinate  
     * @param z Target Z coordinate
     */
    void TeleportPlayer(float x, float y, float z);
    
    /**
     * @brief Spawn a new object in the scene via console
     * @param type Object type to spawn ("cube", "sphere", "wall", etc.)
     * @param x Spawn X coordinate
     * @param y Spawn Y coordinate
     * @param z Spawn Z coordinate
     * @return True if object was spawned successfully
     */
    bool SpawnObject(const std::string& type, float x, float y, float z);
    
    /**
     * @brief Delete an object from the scene by index
     * @param index Index of object to delete
     * @return True if object was deleted successfully
     */
    bool DeleteObject(size_t index);
    
    /**
     * @brief Clear all non-essential objects from the scene
     * @param keepPlayer Whether to preserve the player object
     */
    void ClearScene(bool keepPlayer = true);
    
    /**
     * @brief Set time scale for the entire game simulation
     * @param scale Time scale multiplier (1.0 = normal, 0.5 = half speed, 2.0 = double speed)
     */
    void SetTimeScale(float scale);
    
    /**
     * @brief Get the current time scale
     * @return Current time scale multiplier
     */
    float GetTimeScale() const { return m_timeScale; }

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
    
    // Console integration state
    float m_timeScale{ 1.0f };     ///< Global time scale multiplier for console control
    int m_drawCallCount{ 0 };      ///< Draw call counter for performance monitoring
    int m_triangleCount{ 0 };      ///< Triangle counter for performance monitoring
    bool m_godModeEnabled{ false }; ///< God mode state for console debugging
    bool m_noclipEnabled{ false };  ///< Noclip state for console debugging
    bool m_infiniteAmmoEnabled{ false }; ///< Infinite ammo state for console debugging
};