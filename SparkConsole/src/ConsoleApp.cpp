#include "ConsoleApp.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <algorithm>
#include <conio.h>
#include <iomanip>

ConsoleApp::ConsoleApp() 
    : m_historyIndex(0)
    , m_running(true)
    , m_consoleOutput(GetStdHandle(STD_OUTPUT_HANDLE))
    , m_consoleInput(GetStdHandle(STD_INPUT_HANDLE))
{
    RegisterDefaultCommands();
    
    // Start engine input reading thread
    std::thread engineInputThread(&ConsoleApp::ReadEngineInput, this);
    engineInputThread.detach();
    
    PrintLog(L"Console initialized with engine communication support.");
}

ConsoleApp::~ConsoleApp() {
    m_running = false;
}

void ConsoleApp::Run() {
    system("cls");
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"   Spark Engine Console v1.0.0" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << std::endl;
    PrintLog(L"Console application started. Type 'help' for commands or 'exit' to quit.");

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD fileType = GetFileType(hStdin);
    bool pipeMode = (fileType == FILE_TYPE_PIPE);
    if (pipeMode) {
        PrintLog(L"Connected to Spark Engine via pipe communication.");
    } else {
        PrintLog(L"Running in standalone mode. Engine commands will not be available.");
        PrintLog(L"Waiting for SparkEngine.exe to connect... (or type commands to use standalone)");
    }

    std::string input;
    int noInputCounter = 0;

    // **FIXED: In pipe mode, start a separate thread for keyboard input**
    std::thread keyboardThread;
    std::atomic<bool> keyboardThreadRunning{false};
    
    if (pipeMode) {
        keyboardThreadRunning = true;
        keyboardThread = std::thread([&]() {
            while (keyboardThreadRunning && m_running) {
                if (_kbhit()) {
                    char ch = _getch();
                    if (ch == '\r' || ch == '\n') {
                        if (!input.empty()) {
                            AddToHistory(input);
                            if (input == "exit" || input == "quit") {
                                PrintLog(L"Console shutting down...");
                                m_running = false;
                                keyboardThreadRunning = false;
                                break;
                            }
                            // **Send command to engine via stdout**
                            std::cout << input << std::endl;
                            std::cout.flush();
                            input.clear();
                        }
                        // **Display newline using console API**
                        HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
                        WriteConsoleW(hConsoleOut, L"\n", 1, NULL, NULL);
                    } else if (ch == '\b') {
                        if (!input.empty()) {
                            input.pop_back();
                            HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
                            WriteConsoleW(hConsoleOut, L"\b \b", 3, NULL, NULL);
                        }
                    } else if (ch >= 32 && ch <= 126) {
                        input += ch;
                        HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
                        wchar_t wch = static_cast<wchar_t>(ch);
                        WriteConsoleW(hConsoleOut, &wch, 1, NULL, NULL);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    while (m_running) {
        // **FIXED: In pipe mode, just display prompt and let keyboard thread handle input**
        if (pipeMode) {
            HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(hConsoleOut, &csbi);
            
            SetConsoleTextAttribute(hConsoleOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            WriteConsoleW(hConsoleOut, L"> ", 2, NULL, NULL);
            SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            // **Just sleep and let the keyboard thread and ReadEngineInput thread do their work**
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            noInputCounter++;
            if (noInputCounter > 100) { // Check connection every 10 seconds
                DWORD newFileType = GetFileType(hStdin);
                if (newFileType != FILE_TYPE_PIPE) {
                    PrintLog(L"Engine connection lost. Switching to standalone mode.");
                    pipeMode = false;
                    keyboardThreadRunning = false;
                }
                noInputCounter = 0;
            }
        } else {
            // **Standalone mode - use standard input handling**
            SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "> ";
            SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout.flush();
            
            if (std::getline(std::cin, input)) {
                input.erase(0, input.find_first_not_of(" \t"));
                input.erase(input.find_last_not_of(" \t") + 1);
                if (!input.empty()) {
                    AddToHistory(input);
                    if (input == "exit" || input == "quit") {
                        PrintLog(L"Console shutting down...");
                        m_running = false;
                        break;
                    }
                    ExecuteCommand(input);
                }
            } else {
                PrintLog(L"Input stream closed. Exiting...");
                m_running = false;
                break;
            }
        }
    }
    
    // **Cleanup keyboard thread**
    if (keyboardThreadRunning) {
        keyboardThreadRunning = false;
        if (keyboardThread.joinable()) {
            keyboardThread.join();
        }
    }
    
    PrintLog(L"Console application terminated.");
}

void ConsoleApp::ReadEngineInput() {
    // This method reads messages from the engine via stdin (which is redirected from engine's pipe)
    std::string line;
    char buffer[1024];
    DWORD bytesRead = 0;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    
    // **DEBUG: Add debug output to track what's happening**
    OutputDebugStringA("ReadEngineInput: Starting engine input reader thread\n");
    
    // Check if stdin is redirected (connected to a pipe)
    DWORD fileType = GetFileType(hStdin);
    
    // Try to detect if we're connected to a pipe by testing for pipe-specific operations
    DWORD bytesAvailable = 0;
    BOOL isPipeConnected = PeekNamedPipe(hStdin, NULL, 0, NULL, &bytesAvailable, NULL);
    
    if (isPipeConnected || fileType == FILE_TYPE_PIPE) {
        PrintLog(L"Connected to engine via pipe communication.");
        OutputDebugStringA("ReadEngineInput: Pipe connection confirmed\n");
    } else {
        PrintLog(L"No pipe connection detected. Running in standalone mode.");
        OutputDebugStringA("ReadEngineInput: No pipe connection detected\n");
        return; // Exit thread if no pipe connection
    }
    
    while (m_running) {
        // Check if data is available
        DWORD bytesAvailable = 0;
        BOOL pipeResult = PeekNamedPipe(hStdin, NULL, 0, NULL, &bytesAvailable, NULL);
        
        if (!pipeResult) {
            DWORD error = GetLastError();
            if (error == ERROR_BROKEN_PIPE || error == ERROR_INVALID_HANDLE) {
                PrintLog(L"Engine pipe connection lost.");
                OutputDebugStringA("ReadEngineInput: Pipe connection lost\n");
                break;
            }
            // For other errors, continue trying
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        if (bytesAvailable > 0) {
            // **DEBUG: Log that we're about to read data**
            OutputDebugStringA(("ReadEngineInput: " + std::to_string(bytesAvailable) + " bytes available\n").c_str());
            
            // Read available data
            DWORD bytesToRead = std::min(bytesAvailable, static_cast<DWORD>(sizeof(buffer) - 1));
            if (ReadFile(hStdin, buffer, bytesToRead, &bytesRead, NULL)) {
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    std::string message(buffer);
                    
                    // **DEBUG: Log what we received**
                    std::string debugMsg = "ReadEngineInput: Received data: " + message.substr(0, 100) + "\n";
                    OutputDebugStringA(debugMsg.c_str());
                    
                    // Process each line in the message
                    std::istringstream iss(message);
                    std::string line;
                    while (std::getline(iss, line)) {
                        if (!line.empty()) {
                            // Remove carriage return if present
                            if (!line.empty() && line.back() == '\r') {
                                line.pop_back();
                            }
                            
                            if (!line.empty()) {
                                // **DEBUG: Log each processed line**
                                OutputDebugStringA(("ReadEngineInput: Processing line: " + line + "\n").c_str());
                                
                                // Convert to wide string and display
                                std::wstring wMessage(line.begin(), line.end());
                                PrintEngineLog(wMessage);
                                
                                // Add to message buffer
                                m_messageBuffer.push_back(wMessage);
                                if (m_messageBuffer.size() > MAX_BUFFER_SIZE) {
                                    m_messageBuffer.pop_front();
                                }
                            }
                        }
                    }
                }
            } else {
                DWORD error = GetLastError();
                OutputDebugStringA(("ReadEngineInput: ReadFile failed with error " + std::to_string(error) + "\n").c_str());
                if (error == ERROR_BROKEN_PIPE) {
                    PrintLog(L"Engine connection lost.");
                    break;
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Increased from 10ms to 50ms to reduce CPU usage
    }
    
    OutputDebugStringA("ReadEngineInput: Engine input reader thread terminated\n");
    PrintLog(L"Engine input reader thread terminated.");
}

void ConsoleApp::ReadUserInput() {
    // TODO: Advanced input handling with history navigation
    // This could handle arrow keys for command history
}

void ConsoleApp::PrintLog(const std::wstring& msg) {
    std::lock_guard<std::mutex> lock(m_outputMutex);
    
    // **FIXED: Use Windows Console API instead of wcout to prevent interference**
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Add timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    // Format timestamp
    std::wstringstream timeStr;
    timeStr << std::put_time(std::localtime(&time_t), L"[%H:%M:%S] ");
    
    SetConsoleTextAttribute(hConsoleOut, FOREGROUND_BLUE | FOREGROUND_GREEN);
    DWORD written;
    std::wstring timestamp = timeStr.str();
    WriteConsoleW(hConsoleOut, timestamp.c_str(), static_cast<DWORD>(timestamp.length()), &written, NULL);
    
    SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    WriteConsoleW(hConsoleOut, msg.c_str(), static_cast<DWORD>(msg.length()), &written, NULL);
    WriteConsoleW(hConsoleOut, L"\n", 1, &written, NULL);
    
    // Manage buffer size
    m_messageBuffer.push_back(msg);
    if (m_messageBuffer.size() > MAX_BUFFER_SIZE) {
        m_messageBuffer.pop_front();
    }
}

void ConsoleApp::PrintEngineLog(const std::wstring& msg) {
    std::lock_guard<std::mutex> lock(m_outputMutex);
    
    // **IMPROVED: Better duplicate message filtering**
    static std::wstring lastMessage;
    static int duplicateCount = 0;
    static auto lastLogTime = std::chrono::steady_clock::now();
    
    auto now = std::chrono::steady_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLogTime).count();
    
    // If same message within 200ms, count as duplicate
    if (msg == lastMessage && timeDiff < 200) {
        duplicateCount++;
        if (duplicateCount > 3) { // Reduced from 5 to 3
            return; // Skip excessive duplicate messages more aggressively
        }
    } else {
        if (duplicateCount > 3) {
            // Show summary of skipped messages
            HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN);
            
            auto currentTime = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(currentTime);
            std::wstringstream skipMsg;
            skipMsg << L"[" << std::put_time(std::localtime(&time_t), L"%H:%M:%S") 
                    << L"] ENGINE: (Skipped " << duplicateCount - 3 << L" duplicate messages)\n";
            
            std::wstring skipStr = skipMsg.str();
            DWORD written;
            WriteConsoleW(hConsoleOut, skipStr.c_str(), static_cast<DWORD>(skipStr.length()), &written, NULL);
        }
        duplicateCount = 0;
        lastMessage = msg;
        lastLogTime = now;
    }
    
    // **FIXED: Use Windows Console API instead of wcout**
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    auto currentTime = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(currentTime);
    
    // Format the complete message
    std::wstringstream fullMsg;
    fullMsg << L"[" << std::put_time(std::localtime(&time_t), L"%H:%M:%S") << L"] ENGINE: " << msg << L"\n";
    
    SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wstring fullStr = fullMsg.str();
    DWORD written;
    WriteConsoleW(hConsoleOut, fullStr.c_str(), static_cast<DWORD>(fullStr.length()), &written, NULL);
    
    SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void ConsoleApp::PrintResult(const std::string& result) {
    std::lock_guard<std::mutex> lock(m_outputMutex);
    
    if (!result.empty()) {
        // **FIXED: Use Windows Console API instead of cout to prevent sending to engine**
        HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
        
        SetConsoleTextAttribute(hConsoleOut, FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        // Convert string to wstring for WriteConsoleW
        std::wstring wResult(result.begin(), result.end());
        DWORD written;
        WriteConsoleW(hConsoleOut, wResult.c_str(), static_cast<DWORD>(wResult.length()), &written, NULL);
        WriteConsoleW(hConsoleOut, L"\n", 1, &written, NULL);
        
        SetConsoleTextAttribute(hConsoleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

void ConsoleApp::SetConsoleColor(WORD color) {
    SetConsoleTextAttribute(m_consoleOutput, color);
}

void ConsoleApp::ExecuteCommand(const std::string& cmdLine) {
    if (cmdLine.empty()) return;
    
    // Parse command and arguments
    std::istringstream iss(cmdLine);
    std::string command;
    iss >> command;
    
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    // **FIXED: Check if this is an engine command first, then forward it**
    if (ShouldForwardToEngine(command)) {
        // Forward to engine first
        std::cout << cmdLine << std::endl;
        std::cout.flush();
        PrintResult("Command sent to engine: " + cmdLine);
    } else {
        // Execute local command through registry
        std::string result = m_commandRegistry.ExecuteCommand(command, args);
        PrintResult(result);
    }
}

bool ConsoleApp::ShouldForwardToEngine(const std::string& command) {
    // **EXPANDED: Include all engine commands**
    static const std::vector<std::string> engineCommands = {
        "fps", "info", "test_assert", "test_null_access", "test_assert_not_null",
        "test_assert_range", "crash_mode", "memory_info", "assert_test", "crash_test", "assert_mode",
        "graphics_info", "engine_status", "render_debug", "shader_debug", "test_engine", 
        "minimal_test", "console_status", "quit", "help"  // Add more engine commands
    };
    
    return std::find(engineCommands.begin(), engineCommands.end(), command) != engineCommands.end();
}

void ConsoleApp::RegisterDefaultCommands() {
    // Help command
    m_commandRegistry.RegisterCommand("help", 
        "Show available commands or help for specific command",
        "help [command_name]",
        [this](const std::vector<std::string>& args) -> std::string {
            if (args.empty()) {
                std::stringstream ss;
                ss << "Available Console Commands:\n";
                auto commands = m_commandRegistry.GetAllCommands();
                for (const auto& cmd : commands) {
                    ss << "  " << cmd.name;
                    if (!cmd.description.empty()) {
                        ss << " - " << cmd.description;
                    }
                    ss << "\n";
                }
                ss << "\nEngine Commands (forwarded to engine):\n";
                ss << "  fps - Show current FPS\n";
                ss << "  info - Show engine information\n";
                ss << "  memory_info - Show memory information\n";
                ss << "  test_assert - Trigger test assertion\n";
                ss << "  crash_mode <on|off> - Toggle crash dumps\n";
                ss << "\nType 'help <command>' for detailed information about a command.";
                return ss.str();
            } else {
                return m_commandRegistry.GetCommandHelp(args[0]);
            }
        });
    
    // Clear command
    // **IMPROVED: Clear command with complete refresh**
    m_commandRegistry.RegisterCommand("clear",
        "Clear the console screen and refresh display",
        "clear",
        [this](const std::vector<std::string>& args) -> std::string {
            system("cls");
            
            // Reprint enhanced header
            std::wcout << L"========================================" << std::endl;
            std::wcout << L"   Spark Engine Console v1.0.0" << std::endl;
            std::wcout << L"   Console Refreshed" << std::endl;
            std::wcout << L"========================================" << std::endl;
            std::wcout << std::endl;
            
            // Show connection status
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD fileType = GetFileType(hStdin);
            if (fileType == FILE_TYPE_PIPE) {
                std::wcout << L"? Connected to Spark Engine via pipe" << std::endl;
            } else {
                std::wcout << L"? Running in standalone mode" << std::endl;
            }
            
            std::wcout << L"Type 'help' for available commands" << std::endl;
            std::wcout << L"Type 'info' to test engine connection" << std::endl;
            std::wcout << std::endl;
            std::wcout.flush();
            
            return "";
        });
    
    // History command
    m_commandRegistry.RegisterCommand("history",
        "Show command history",
        "history",
        [this](const std::vector<std::string>& args) -> std::string {
            std::lock_guard<std::mutex> lock(m_historyMutex);
            std::stringstream ss;
            ss << "Command History:\n";
            for (size_t i = 0; i < m_commandHistory.size(); ++i) {
                ss << "  " << (i + 1) << ": " << m_commandHistory[i] << "\n";
            }
            return ss.str();
        });
    
    // Info command
    m_commandRegistry.RegisterCommand("status",
        "Show console status information",
        "status",
        [this](const std::vector<std::string>& args) -> std::string {
            std::stringstream ss;
            ss << "Spark Engine Debug Console\n";
            ss << "Version: 1.0.0\n";
            ss << "Commands registered: " << m_commandRegistry.GetAllCommands().size() << "\n";
            ss << "History entries: " << m_commandHistory.size() << "\n";
            ss << "Buffer size: " << m_messageBuffer.size() << "/" << MAX_BUFFER_SIZE << "\n";
            ss << "Connection status: " << (m_running ? "Active" : "Disconnected");
            return ss.str();
        });
    
    // Echo command for testing
    m_commandRegistry.RegisterCommand("echo",
        "Echo back the provided arguments",
        "echo <message>",
        [](const std::vector<std::string>& args) -> std::string {
            std::stringstream ss;
            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) ss << " ";
                ss << args[i];
            }
            return ss.str();
        });
        
    // Test connection command
    m_commandRegistry.RegisterCommand("test_connection",
        "Test connection to engine",
        "test_connection",
        [this](const std::vector<std::string>& args) -> std::string {
            // Send a test command to engine
            std::cout << "info" << std::endl;
            std::cout.flush();
            return "Test command sent to engine. Check for response above.";
        });
        
    // Simple diagnostic command
    m_commandRegistry.RegisterCommand("diag",
        "Show console diagnostic information",
        "diag",
        [this](const std::vector<std::string>& args) -> std::string {
            std::stringstream ss;
            ss << "SparkConsole Diagnostics:\n";
            ss << "  Console running: " << (m_running ? "Yes" : "No") << "\n";
            ss << "  Commands registered: " << m_commandRegistry.GetAllCommands().size() << "\n";
            ss << "  Message buffer size: " << m_messageBuffer.size() << "/" << MAX_BUFFER_SIZE << "\n";
            
            // Check stdin type
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD fileType = GetFileType(hStdin);
            ss << "  Input mode: ";
            switch (fileType) {
                case FILE_TYPE_CHAR: ss << "Character device"; break;
                case FILE_TYPE_DISK: ss << "Disk file"; break;
                case FILE_TYPE_PIPE: ss << "Named pipe (connected to engine)"; break;
                case FILE_TYPE_REMOTE: ss << "Remote"; break;
                case FILE_TYPE_UNKNOWN: 
                default: ss << "Unknown (" << fileType << ")"; break;
            }
            ss << "\n";
            
            // Check if we can detect SparkEngine.exe
            ss << "  Looking for SparkEngine.exe in current directory: ";
            ss << (std::filesystem::exists("SparkEngine.exe") ? "Found" : "Not found");
            
            return ss.str();
        });
    
    // **NEW: Add pipe test command for debugging**
    m_commandRegistry.RegisterCommand("pipe_test",
        "Test pipe communication with engine",
        "pipe_test",
        [this](const std::vector<std::string>& args) -> std::string {
            // Check stdin type
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD fileType = GetFileType(hStdin);
            
            std::stringstream ss;
            ss << "Pipe Communication Test:\n";
            ss << "  Stdin file type: ";
            
            switch (fileType) {
                case FILE_TYPE_CHAR: ss << "Character device (no pipe)"; break;
                case FILE_TYPE_DISK: ss << "Disk file"; break;
                case FILE_TYPE_PIPE: ss << "Named pipe (connected!)"; break;
                case FILE_TYPE_REMOTE: ss << "Remote"; break;
                case FILE_TYPE_UNKNOWN: 
                default: ss << "Unknown (" << fileType << ")"; break;
            }
            ss << "\n";
            
            // Test sending a command to engine
            if (fileType == FILE_TYPE_PIPE) {
                ss << "  Sending test command to engine...";
                std::cout << "test_engine" << std::endl;
                std::cout.flush();
                ss << " Sent!\n";
                ss << "  Watch for response from engine above.";
            } else {
                ss << "  No pipe connection - cannot send commands to engine";
            }
            
            return ss.str();
        });
    
    // **NEW: Add console refresh command**
    m_commandRegistry.RegisterCommand("refresh",
        "Refresh console display",
        "refresh",
        [this](const std::vector<std::string>& args) -> std::string {
            // Force console refresh
            std::wcout << std::endl;
            SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::wcout << L"Console display refreshed." << std::endl;
            SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::wcout.flush();
            return "";
        });
}

void ConsoleApp::AddToHistory(const std::string& cmd) {
    std::lock_guard<std::mutex> lock(m_historyMutex);
    
    // Don't add duplicate consecutive commands
    if (!m_commandHistory.empty() && m_commandHistory.back() == cmd) {
        return;
    }
    
    m_commandHistory.push_back(cmd);
    
    // Limit history size
    const size_t MAX_HISTORY = 100;
    if (m_commandHistory.size() > MAX_HISTORY) {
        m_commandHistory.erase(m_commandHistory.begin());
    }
    
    m_historyIndex = m_commandHistory.size();
}

std::string ConsoleApp::GetPreviousCommand() {
    std::lock_guard<std::mutex> lock(m_historyMutex);
    
    if (m_commandHistory.empty() || m_historyIndex == 0) {
        return "";
    }
    
    --m_historyIndex;
    return m_commandHistory[m_historyIndex];
}

std::string ConsoleApp::GetNextCommand() {
    std::lock_guard<std::mutex> lock(m_historyMutex);
    
    if (m_commandHistory.empty() || m_historyIndex >= m_commandHistory.size() - 1) {
        m_historyIndex = m_commandHistory.size();
        return "";
    }
    
    ++m_historyIndex;
    return m_commandHistory[m_historyIndex];
}

void ConsoleApp::ClearInputLine() {
    // TODO: Implement input line clearing for advanced input handling
}

void ConsoleApp::UpdateInputLine(const std::string& text) {
    // TODO: Implement input line updating for advanced input handling
}