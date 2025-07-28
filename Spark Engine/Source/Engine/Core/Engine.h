#pragma once
#include <memory>
#include <string>
#include <Windows.h>

class Renderer;
class AssetManager;
class SceneManager;
class EditorManager;
class AngelScriptEngine;
class InputManager;

struct EngineConfig {
    bool enableEditor = true;
    std::string projectPath = "Assets/";
    int windowWidth = 1280;
    int windowHeight = 720;
    std::string windowTitle = "Game Engine";
};

class Engine {
public:
    static Engine& Instance();
    
    bool Initialize(const EngineConfig& config);
    void Run();
    void Shutdown();
    
    Renderer* GetRenderer() const { return m_renderer.get(); }
    AssetManager* GetAssetManager() const { return m_assetManager.get(); }
    SceneManager* GetSceneManager() const { return m_sceneManager.get(); }
    EditorManager* GetEditor() const { return m_editor.get(); }
    AngelScriptEngine* GetAngelScriptEngine() const { return m_angelScript.get(); }
    InputManager* GetInput() const { return m_input.get(); }
    
    bool IsEditor() const { return m_config.enableEditor; }
    bool IsPlaying() const { return m_isPlaying; }
    void SetPlaying(bool playing) { m_isPlaying = playing; }
    
    HWND GetWindowHandle() const { return m_hwnd; }
    
private:
    Engine() = default;
    
    bool InitializeWindow();
    void Update(float deltaTime);
    void Render();
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<AssetManager> m_assetManager;
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<EditorManager> m_editor;
    std::unique_ptr<AngelScriptEngine> m_angelScript;
    std::unique_ptr<InputManager> m_input;
    
    EngineConfig m_config;
    HWND m_hwnd = nullptr;
    bool m_isPlaying = false;
    bool m_running = true;
};

