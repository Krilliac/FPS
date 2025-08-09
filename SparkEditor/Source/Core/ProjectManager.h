/**
 * @file ProjectManager.h
 * @brief Project management system for the Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file defines the project management system that handles project
 * creation, loading, saving, and organization within the editor.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SparkEditor {

/**
 * @brief Project information structure
 */
struct ProjectInfo {
    std::string name;               ///< Project name
    std::string path;               ///< Project root path
    std::string version;            ///< Project version
    std::string description;        ///< Project description
    std::vector<std::string> scenes; ///< Scene file paths
    std::string lastOpenedScene;    ///< Last opened scene path
    uint64_t lastModified = 0;      ///< Last modified timestamp
};

/**
 * @brief Project management system
 * 
 * Handles project creation, loading, saving, and organization for the editor.
 */
class ProjectManager {
public:
    /**
     * @brief Constructor
     */
    ProjectManager();

    /**
     * @brief Destructor
     */
    ~ProjectManager();

    /**
     * @brief Initialize the project manager
     * @return true if initialization succeeded, false otherwise
     */
    bool Initialize();

    /**
     * @brief Shutdown the project manager
     */
    void Shutdown();

    /**
     * @brief Create a new project
     * @param projectName Name of the new project
     * @param projectPath Directory where to create the project
     * @return true if project was created successfully
     */
    bool CreateProject(const std::string& projectName, const std::string& projectPath);

    /**
     * @brief Open an existing project
     * @param projectPath Path to the project file
     * @return true if project was opened successfully
     */
    bool OpenProject(const std::string& projectPath);

    /**
     * @brief Save current project
     * @return true if project was saved successfully
     */
    bool SaveProject();

    /**
     * @brief Close current project
     */
    void CloseProject();

    /**
     * @brief Check if a project is currently open
     * @return true if a project is open
     */
    bool HasOpenProject() const { return m_hasOpenProject; }

    /**
     * @brief Get current project information
     * @return Reference to current project info
     */
    const ProjectInfo& GetCurrentProject() const { return m_currentProject; }

    /**
     * @brief Get recent projects list
     * @return Vector of recent project paths
     */
    const std::vector<std::string>& GetRecentProjects() const { return m_recentProjects; }

    /**
     * @brief Add project to recent projects list
     * @param projectPath Path to the project
     */
    void AddToRecentProjects(const std::string& projectPath);

private:
    /**
     * @brief Load project settings from file
     * @param projectPath Path to the project file
     * @return true if loading succeeded
     */
    bool LoadProjectSettings(const std::string& projectPath);

    /**
     * @brief Save project settings to file
     * @return true if saving succeeded
     */
    bool SaveProjectSettings();

    /**
     * @brief Create default project structure
     * @param projectPath Project root directory
     * @return true if creation succeeded
     */
    bool CreateProjectStructure(const std::string& projectPath);

private:
    ProjectInfo m_currentProject;                   ///< Current project information
    std::vector<std::string> m_recentProjects;     ///< Recent projects list
    bool m_hasOpenProject = false;                  ///< Whether a project is open
    bool m_isInitialized = false;                   ///< Initialization state
};

} // namespace SparkEditor