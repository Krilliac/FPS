#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

class GraphicsEngine;
class InputManager;
class GameObject;

class SceneManager
{
public:
    SceneManager(GraphicsEngine* graphics, InputManager* input);
    ~SceneManager() = default;

    // Load a .scene file and populate objects
    bool LoadScene(const std::wstring& filepath);

    // Access the created objects
    const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

private:
    // Custom .scene loader (text-based)
    bool LoadCustom(const std::wstring& path);

    GraphicsEngine* m_graphics;
    InputManager* m_input;
    std::vector<std::unique_ptr<GameObject>>    m_objects;
};