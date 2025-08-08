#include "ConsoleProcessManager.h"
#include "Assert.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <algorithm>

namespace Spark {

ConsoleProcessManager& ConsoleProcessManager::GetInstance() {
    static ConsoleProcessManager instance;
    return instance;
}

// **NEW: Global accessor function for Assert system**
ConsoleProcessManager& GetConsoleProcessManagerInstance() {
    return ConsoleProcessManager::GetInstance();
}

ConsoleProcessManager::ConsoleProcessManager() 
    : m_commandRegistry(std::make_unique<CommandRegistry>()) {
    
    // Register default commands
    m_commandRegistry->RegisterCommand("help", 
        [this](const std::vector<std::string>& args) -> std::string {
            std::stringstream ss;
            ss << "Available commands:\n";
            auto commands = m_commandRegistry->GetAllCommands();
            for (const auto& cmd : commands) {
                ss << "  " << cmd.name;
                if (!cmd.description.empty()) {
                    ss << " - " << cmd.description;
                }
                ss << "\n";
                if (!cmd.usage.empty()) {
                    ss << "    Usage: " << cmd.usage << "\n";
                }
            }
            return ss.str();
        },
        "Show available commands",
        "help"
    );
    
    m_commandRegistry->RegisterCommand("quit", 
        [](const std::vector<std::string>& args) -> std::string {
            PostQuitMessage(0);
            return "Shutting down engine...";
        },
        "Quit the application",
        "quit"
    );

    // **NEW: Assert and crash handling commands**
    m_commandRegistry->RegisterCommand("assert_test", 
        [](const std::vector<std::string>& args) -> std::string {
            // Trigger a test assertion
            ASSERT_MSG(false, "Test assertion triggered from console command");
            return "This should not be reached";
        },
        "Trigger a test assertion",
        "assert_test"
    );

    m_commandRegistry->RegisterCommand("crash_test", 
        [](const std::vector<std::string>& args) -> std::string {
            // Trigger a test crash
            int* nullPtr = nullptr;
            *nullPtr = 42; // This will cause an access violation
            return "This should not be reached";
        },
        "Trigger a test crash",
        "crash_test"
    );

    m_commandRegistry->RegisterCommand("assert_mode", 
        [](const std::vector<std::string>& args) -> std::string {
            if (args.empty()) {
                return "Usage: assert_mode <on|off>\n"
                       "Controls whether assertions trigger crash dumps";
            }
            
            std::string mode = args[0];
            std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
            
            if (mode == "on" || mode == "true" || mode == "1") {
                extern void SetAssertCrashBehavior(bool);
                SetAssertCrashBehavior(true);
                return "Assert crash dumps enabled";
            } else if (mode == "off" || mode == "false" || mode == "0") {
                extern void SetAssertCrashBehavior(bool);
                SetAssertCrashBehavior(false);
                return "Assert crash dumps disabled";
            } else {
                return "Invalid mode. Use: on, off, true, false, 1, or 0";
            }
        },
        "Enable/disable crash dumps for assertions",
        "assert_mode <on|off>"
    );
}

ConsoleProcessManager::~ConsoleProcessManager() {
    Shutdown();
}

bool ConsoleProcessManager::Initialize(const std::wstring& consolePath) {
    if (m_initialized) {
        return true;
    }
    
    // Try to find SparkConsole.exe in various locations
    std::vector<std::wstring> searchPaths = {
        consolePath,
        L"bin\\SparkConsole.exe",
        L"..\\bin\\SparkConsole.exe",
        L"SparkConsole\\bin\\SparkConsole.exe"
    };
    
    std::wstring actualPath;
    for (const auto& path : searchPaths) {
        if (std::filesystem::exists(path)) {
            actualPath = path;
            break;
        }
    }
    
    if (actualPath.empty()) {
        // Console not found, fall back to OutputDebugString
        m_initialized = true;
        Log(L"External console not found - using fallback logging", L"WARNING");
        return true;
    }
    
    bool success = LaunchConsoleProcess(actualPath);
    m_initialized = true;
    
    if (success) {
        Log(L"External console system initialized successfully", L"INFO");
        Log(L"Assert and crash logging integrated with external console", L"INFO");
    }
    
    return success;
}

void ConsoleProcessManager::Shutdown() {
    if (!m_initialized) {
        return;
    }
    
    m_consoleRunning = false;
    
    // Close pipe handles
    if (m_stdInWrite) {
        CloseHandle(m_stdInWrite);
        m_stdInWrite = NULL;
    }
    if (m_stdOutRead) {
        CloseHandle(m_stdOutRead);
        m_stdOutRead = NULL;
    }
    if (m_stdInRead) {
        CloseHandle(m_stdInRead);
        m_stdInRead = NULL;
    }
    if (m_stdOutWrite) {
        CloseHandle(m_stdOutWrite);
        m_stdOutWrite = NULL;
    }
    
    // Terminate console process
    if (m_processHandle) {
        TerminateProcess(m_processHandle, 0);
        WaitForSingleObject(m_processHandle, 1000);
        CloseHandle(m_processHandle);
        m_processHandle = NULL;
    }
    
    if (m_threadHandle) {
        CloseHandle(m_threadHandle);
        m_threadHandle = NULL;
    }
    
    m_initialized = false;
}

void ConsoleProcessManager::Log(const std::wstring& message, const std::wstring& type) {
    std::wstring formattedMessage = L"[" + type + L"] " + message;
    
    if (m_consoleRunning && m_stdInWrite) {
        // Queue message for console process
        {
            std::lock_guard<std::mutex> lock(m_messageMutex);
            m_messageQueue.push(formattedMessage);
        }
        
        // Try to write immediately
        WriteToConsole(formattedMessage);
    } else {
        // Fallback to OutputDebugString
        OutputDebugStringW(formattedMessage.c_str());
        OutputDebugStringW(L"\n");
    }
}

// **NEW: Special logging method for crashes**
void ConsoleProcessManager::LogCrash(const std::string& crashInfo) {
    // Convert crash info to wide string
    int len = MultiByteToWideChar(CP_UTF8, 0, crashInfo.c_str(), -1, nullptr, 0);
    if (len > 0) {
        std::wstring wCrashInfo(len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, crashInfo.c_str(), -1, &wCrashInfo[0], len);
        wCrashInfo.pop_back(); // Remove null terminator
        
        Log(wCrashInfo, L"CRASH");
        
        // Also try to flush any pending messages
        ProcessCommands();
    }
}

void ConsoleProcessManager::ProcessCommands() {
    if (!m_consoleRunning) {
        return;
    }
    
    // Read any pending commands from console
    ReadFromConsole();
    
    // Process queued commands
    std::queue<std::string> commandsToProcess;
    {
        std::lock_guard<std::mutex> lock(m_commandMutex);
        commandsToProcess.swap(m_commandQueue);
    }
    
    while (!commandsToProcess.empty()) {
        std::string command = commandsToProcess.front();
        commandsToProcess.pop();
        
        try {
            std::string result = m_commandRegistry->ExecuteCommand(command);
            if (!result.empty()) {
                // Send result back to console
                std::wstring wResult(result.begin(), result.end());
                Log(wResult, L"RESULT");
            }
        } catch (const std::exception& e) {
            std::string error = "Command error: " + std::string(e.what());
            std::wstring wError(error.begin(), error.end());
            Log(wError, L"ERROR");
        }
    }
}

void ConsoleProcessManager::RegisterCommand(const std::string& name, 
                                          std::function<std::string(const std::vector<std::string>&)> handler,
                                          const std::string& description,
                                          const std::string& usage) {
    m_commandRegistry->RegisterCommand(name, handler, description, usage);
}

bool ConsoleProcessManager::LaunchConsoleProcess(const std::wstring& path) {
    // Create pipes for communication
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    // Create pipes
    if (!CreatePipe(&m_stdOutRead, &m_stdOutWrite, &sa, 0) ||
        !CreatePipe(&m_stdInRead, &m_stdInWrite, &sa, 0)) {
        return false;
    }
    
    // Ensure the read handles are not inherited
    SetHandleInformation(m_stdOutRead, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(m_stdInWrite, HANDLE_FLAG_INHERIT, 0);
    
    // Set up process startup info
    STARTUPINFOW si = {};
    si.cb = sizeof(STARTUPINFOW);
    si.hStdError = m_stdOutWrite;
    si.hStdOutput = m_stdOutWrite;
    si.hStdInput = m_stdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    PROCESS_INFORMATION pi = {};
    
    // Create the console process
    BOOL success = CreateProcessW(
        path.c_str(),          // Application name
        NULL,                  // Command line
        NULL,                  // Process security attributes
        NULL,                  // Primary thread security attributes
        TRUE,                  // Handles are inherited
        CREATE_NEW_CONSOLE,    // Creation flags
        NULL,                  // Use parent's environment
        NULL,                  // Use parent's current directory
        &si,                   // STARTUPINFO
        &pi                    // PROCESS_INFORMATION
    );
    
    if (!success) {
        CloseHandle(m_stdOutRead);
        CloseHandle(m_stdOutWrite);
        CloseHandle(m_stdInRead);
        CloseHandle(m_stdInWrite);
        return false;
    }
    
    m_processHandle = pi.hProcess;
    m_threadHandle = pi.hThread;
    m_consoleRunning = true;
    
    // Close handles we don't need
    CloseHandle(m_stdOutWrite);
    CloseHandle(m_stdInRead);
    m_stdOutWrite = NULL;
    m_stdInRead = NULL;
    
    return true;
}

bool ConsoleProcessManager::ReadFromConsole() {
    if (!m_stdOutRead) {
        return false;
    }
    
    DWORD bytesAvailable = 0;
    if (!PeekNamedPipe(m_stdOutRead, NULL, 0, NULL, &bytesAvailable, NULL)) {
        return false;
    }
    
    if (bytesAvailable == 0) {
        return true; // No data available
    }
    
    char buffer[1024];
    DWORD bytesRead = 0;
    if (!ReadFile(m_stdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        return false;
    }
    
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string command(buffer);
        
        // Remove trailing newlines
        while (!command.empty() && (command.back() == '\n' || command.back() == '\r')) {
            command.pop_back();
        }
        
        if (!command.empty()) {
            std::lock_guard<std::mutex> lock(m_commandMutex);
            m_commandQueue.push(command);
        }
    }
    
    return true;
}

bool ConsoleProcessManager::WriteToConsole(const std::wstring& message) {
    if (!m_stdInWrite) {
        return false;
    }
    
    // Convert to UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, message.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Size <= 0) {
        return false;
    }
    
    std::string utf8Message(utf8Size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, message.c_str(), -1, &utf8Message[0], utf8Size, NULL, NULL);
    utf8Message.pop_back(); // Remove null terminator
    utf8Message += "\n";
    
    DWORD bytesWritten = 0;
    return WriteFile(m_stdInWrite, utf8Message.c_str(), (DWORD)utf8Message.length(), &bytesWritten, NULL);
}

// CommandRegistry implementation
void CommandRegistry::RegisterCommand(const std::string& name, CommandHandler handler,
                                    const std::string& description,
                                    const std::string& usage) {
    CommandInfo info;
    info.name = name;
    info.handler = handler;
    info.description = description;
    info.usage = usage;
    
    m_commands[name] = info;
}

std::string CommandRegistry::ExecuteCommand(const std::string& commandLine) {
    auto args = ParseArguments(commandLine);
    if (args.empty()) {
        return "Empty command";
    }
    
    std::string commandName = args[0];
    args.erase(args.begin()); // Remove command name from args
    
    auto it = m_commands.find(commandName);
    if (it == m_commands.end()) {
        return "Unknown command: " + commandName;
    }
    
    try {
        return it->second.handler(args);
    } catch (const std::exception& e) {
        return "Command execution error: " + std::string(e.what());
    }
}

std::vector<CommandRegistry::CommandInfo> CommandRegistry::GetAllCommands() const {
    std::vector<CommandInfo> result;
    for (const auto& pair : m_commands) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::string> CommandRegistry::ParseArguments(const std::string& commandLine) {
    std::vector<std::string> args;
    std::istringstream iss(commandLine);
    std::string arg;
    
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    return args;
}

} // namespace Spark