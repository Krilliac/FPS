/**
 * @file main.cpp
 * @brief Entry point for the Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file contains the main entry point for the Spark Engine Editor application.
 * The editor provides a comprehensive development environment for creating games
 * and interactive content using the Spark Engine.
 * 
 * Features:
 * - Professional UI with dockable panels
 * - Real-time scene editing and visualization
 * - Asset management and import pipeline
 * - Live engine integration and debugging
 * - Visual scripting and material editing
 * - Terrain and environment tools
 */

#include "Core/EditorApplication.h"
#include <Windows.h>
#include <iostream>
#include <memory>
#include <exception>

/**
 * @brief Windows application entry point
 * 
 * Initializes the Spark Engine Editor application with proper Windows integration,
 * error handling, and resource management.
 * 
 * @param hInstance Handle to the current instance of the application
 * @param hPrevInstance Handle to the previous instance (always NULL in Win32)
 * @param lpCmdLine Command line arguments as a string
 * @param nCmdShow How the window should be displayed
 * @return Exit code (0 for success, non-zero for failure)
 */
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Suppress unused parameter warnings
    UNREFERENCED_PARAMETER(hPrevInstance);
    
    // Enable console for debugging in development builds
#ifdef _DEBUG
    if (AllocConsole()) {
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        SetConsoleTitleW(L"Spark Engine Editor - Debug Console");
        
        std::cout << "Spark Engine Editor Debug Console\n";
        std::cout << "==================================\n";
        std::cout << "Build: " << __DATE__ << " " << __TIME__ << "\n";
        std::cout << "Platform: " << sizeof(void*) * 8 << "-bit\n";
        std::cout << "==================================\n\n";
    }
#endif

    try {
        // Parse command line arguments
        std::string projectPath;
        std::string scenePath;
        bool enableProfiling = false;
        bool verboseLogging = false;
        
        // Simple command line parsing
        std::string cmdLine(lpCmdLine);
        if (cmdLine.find("--project=") != std::string::npos) {
            size_t start = cmdLine.find("--project=") + 10;
            size_t end = cmdLine.find(" ", start);
            if (end == std::string::npos) end = cmdLine.length();
            projectPath = cmdLine.substr(start, end - start);
        }
        
        if (cmdLine.find("--scene=") != std::string::npos) {
            size_t start = cmdLine.find("--scene=") + 8;
            size_t end = cmdLine.find(" ", start);
            if (end == std::string::npos) end = cmdLine.length();
            scenePath = cmdLine.substr(start, end - start);
        }
        
        enableProfiling = (cmdLine.find("--profile") != std::string::npos);
        verboseLogging = (cmdLine.find("--verbose") != std::string::npos);
        
        // Create editor application configuration
        SparkEditor::EditorConfig config;
        config.hInstance = hInstance;
        config.nCmdShow = nCmdShow;
        config.projectPath = projectPath;
        config.scenePath = scenePath;
        config.enableProfiling = enableProfiling;
        config.verboseLogging = verboseLogging;
        
        // Initialize and run the editor application
        auto editorApp = std::make_unique<SparkEditor::EditorApplication>();
        
        if (!editorApp->Initialize(config)) {
            MessageBoxW(nullptr, L"Failed to initialize Spark Engine Editor", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }
        
        std::cout << "Spark Engine Editor initialized successfully\n";
        std::cout << "Starting main application loop...\n\n";
        
        // Run the main application loop
        int exitCode = editorApp->Run();
        
        std::cout << "\nEditor application shutting down...\n";
        
        // Explicit cleanup
        editorApp.reset();
        
        std::cout << "Shutdown complete. Exit code: " << exitCode << "\n";
        
        return exitCode;
        
    } catch (const std::exception& e) {
        std::string errorMsg = "Unhandled exception in editor: ";
        errorMsg += e.what();
        
        std::cerr << errorMsg << std::endl;
        
        // Convert to wide string for MessageBox
        std::wstring wErrorMsg(errorMsg.begin(), errorMsg.end());
        MessageBoxW(nullptr, wErrorMsg.c_str(), L"Critical Error", MB_OK | MB_ICONERROR);
        
        return -2;
        
    } catch (...) {
        const char* errorMsg = "Unknown critical error occurred in editor";
        std::cerr << errorMsg << std::endl;
        MessageBoxW(nullptr, L"Unknown critical error occurred in editor", L"Critical Error", MB_OK | MB_ICONERROR);
        
        return -3;
    }
}

/**
 * @brief Alternative entry point for console applications
 * 
 * Provides a fallback entry point that can be used for command-line tools
 * or when running the editor in console mode.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit code (0 for success, non-zero for failure)
 */
int main(int argc, char* argv[])
{
    // Convert arguments to Windows format and call WinMain
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    
    // Reconstruct command line
    std::string cmdLine;
    for (int i = 1; i < argc; ++i) {
        if (i > 1) cmdLine += " ";
        cmdLine += argv[i];
    }
    
    return WinMain(hInstance, nullptr, const_cast<char*>(cmdLine.c_str()), SW_SHOWDEFAULT);
}