#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <deque>
#include <mutex>

namespace Spark {

class SimpleConsole {
public:
    using CommandHandler = std::function<std::string(const std::vector<std::string>&)>;
    
    struct LogEntry {
        std::string message;
        std::string type;
        std::string timestamp;
    };
    
    struct CommandInfo {
        CommandHandler handler;
        std::string description;
    };

private:
    HWND m_consoleWindow = nullptr;
    HANDLE m_consoleOutput = nullptr;
    HANDLE m_consoleInput = nullptr;
    
    std::unordered_map<std::string, CommandInfo> m_commands;
    std::deque<LogEntry> m_logHistory;
    std::deque<std::string> m_commandHistory;
    
    bool m_initialized = false;
    bool m_visible = true;
    std::string m_currentInput;
    int m_historyIndex = 0;
    
    mutable std::mutex m_logMutex;
    
    enum class Color {
        White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
        Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

public:
    static SimpleConsole& GetInstance();
    
    bool Initialize();
    void Shutdown();
    void Update();
    
    void Log(const std::string& message, const std::string& type = "INFO");
    void LogInfo(const std::string& message);
    void LogWarning(const std::string& message);
    void LogError(const std::string& message);
    void LogSuccess(const std::string& message);
    void LogCritical(const std::string& message);
    void LogTrace(const std::string& message);
    
    void RegisterCommand(const std::string& name, CommandHandler handler, const std::string& description = "");
    bool ExecuteCommand(const std::string& commandLine);
    
    void Show();
    void Hide();
    void Toggle();
    bool IsVisible() const { return m_visible; }
    void Clear();
    
    std::vector<LogEntry> GetLogHistory() const;
    std::vector<std::string> GetCommandHistory() const;

private:
    SimpleConsole() = default;
    ~SimpleConsole() = default;
    SimpleConsole(const SimpleConsole&) = delete;
    SimpleConsole& operator=(const SimpleConsole&) = delete;
    
    void SetColor(Color color);
    void Print(const std::string& text, Color color = Color::White);
    void PrintLine(const std::string& text, Color color = Color::White);
    void ProcessInput();
    void DisplayPrompt();
    void RedrawConsole();
    std::vector<std::string> ParseCommand(const std::string& commandLine);
    std::string GetTimestamp();
    void RegisterDefaultCommands();
    void RegisterAdvancedCommands();
    bool CreateConsoleWindow();
    void SetupConsoleHandles();
    
    // Advanced command registration methods
    void RegisterEngineCommands();
    void RegisterGraphicsCommands();
    void RegisterGameCommands();
    void RegisterPlayerCommands();
    void RegisterPhysicsCommands();
    void RegisterCameraCommands();
    void RegisterSystemCommands();
    void RegisterDebugCommands();
    void RegisterFileCommands();
    void RegisterPerformanceCommands();
    
    // Specialized command categories
    void RegisterRenderingCommands();      // Advanced rendering controls
    void RegisterAudioCommands();          // Audio system management
    void RegisterNetworkingCommands();     // Network simulation and control
    void RegisterProfilingCommands();      // Performance profiling tools
    void RegisterSceneCommands();          // Scene management and manipulation
    void RegisterInputCommands();          // Input system configuration
    void RegisterConfigCommands();         // Configuration management
    void RegisterTestingCommands();        // System testing and benchmarking
};

} // namespace Spark