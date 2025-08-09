/**
 * @file EditorApplication.h
 * @brief Main SparkEditor application class
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include <string>
#include <memory>

namespace SparkEditor {

/**
 * @brief Configuration for the editor
 */
struct EditorConfig {
    std::string projectPath = ".";
    std::string layoutDirectory = "Layouts";
    std::string logDirectory = "Logs";
    bool enableLogging = true;
    bool startMaximized = true;
    float autoSaveInterval = 30.0f;
};

/**
 * @brief Main SparkEditor application class
 */
class EditorApplication {
public:
    EditorApplication();
    ~EditorApplication();

    bool Initialize(const EditorConfig& config);
    int Run();
    void Shutdown();

    bool IsRunning() const { return m_isRunning; }
    void RequestExit() { m_isRunning = false; }

private:
    EditorConfig m_config;
    bool m_isInitialized = false;
    bool m_isRunning = false;
};

} // namespace SparkEditor
