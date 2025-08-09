/**
 * @file ProjectManager.cpp
 * @brief Implementation of the project management system
 * @author Spark Engine Team
 * @date 2025
 */

#include "ProjectManager.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace SparkEditor {

ProjectManager::ProjectManager() = default;

ProjectManager::~ProjectManager() = default;

bool ProjectManager::Initialize()
{
    std::cout << "ProjectManager::Initialize()\n";
    m_isInitialized = true;
    return true;
}

void ProjectManager::Shutdown()
{
    std::cout << "ProjectManager::Shutdown()\n";
    CloseProject();
    m_isInitialized = false;
}

bool ProjectManager::CreateProject(const std::string& projectName, const std::string& projectPath)
{
    std::cout << "Creating project: " << projectName << " at " << projectPath << "\n";
    
    try {
        // Create project directory structure
        if (!CreateProjectStructure(projectPath)) {
            return false;
        }
        
        // Initialize project info
        m_currentProject.name = projectName;
        m_currentProject.path = projectPath;
        m_currentProject.version = "1.0.0";
        m_currentProject.description = "Spark Engine Project";
        m_currentProject.lastModified = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        // Save project settings
        if (!SaveProjectSettings()) {
            return false;
        }
        
        m_hasOpenProject = true;
        AddToRecentProjects(projectPath);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating project: " << e.what() << "\n";
        return false;
    }
}

bool ProjectManager::OpenProject(const std::string& projectPath)
{
    std::cout << "Opening project: " << projectPath << "\n";
    
    if (!LoadProjectSettings(projectPath)) {
        return false;
    }
    
    m_hasOpenProject = true;
    AddToRecentProjects(projectPath);
    
    return true;
}

bool ProjectManager::SaveProject()
{
    if (!m_hasOpenProject) {
        std::cerr << "No project is currently open\n";
        return false;
    }
    
    std::cout << "Saving project: " << m_currentProject.name << "\n";
    return SaveProjectSettings();
}

void ProjectManager::CloseProject()
{
    if (m_hasOpenProject) {
        std::cout << "Closing project: " << m_currentProject.name << "\n";
        m_hasOpenProject = false;
        m_currentProject = ProjectInfo{}; // Reset to default
    }
}

void ProjectManager::AddToRecentProjects(const std::string& projectPath)
{
    // Remove if already exists
    auto it = std::find(m_recentProjects.begin(), m_recentProjects.end(), projectPath);
    if (it != m_recentProjects.end()) {
        m_recentProjects.erase(it);
    }
    
    // Add to front
    m_recentProjects.insert(m_recentProjects.begin(), projectPath);
    
    // Limit to 10 recent projects
    if (m_recentProjects.size() > 10) {
        m_recentProjects.resize(10);
    }
}

bool ProjectManager::LoadProjectSettings(const std::string& projectPath)
{
    std::string settingsFile = projectPath + "/project.json";
    
    // For now, just create a basic project info
    // TODO: Implement proper JSON loading
    m_currentProject.name = std::filesystem::path(projectPath).filename().string();
    m_currentProject.path = projectPath;
    m_currentProject.version = "1.0.0";
    m_currentProject.description = "Loaded project";
    
    return true;
}

bool ProjectManager::SaveProjectSettings()
{
    std::string settingsFile = m_currentProject.path + "/project.json";
    
    try {
        std::ofstream file(settingsFile);
        if (!file.is_open()) {
            std::cerr << "Failed to open project settings file for writing\n";
            return false;
        }
        
        // Write basic JSON (TODO: Use proper JSON library)
        file << "{\n";
        file << "  \"name\": \"" << m_currentProject.name << "\",\n";
        file << "  \"version\": \"" << m_currentProject.version << "\",\n";
        file << "  \"description\": \"" << m_currentProject.description << "\"\n";
        file << "}\n";
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving project settings: " << e.what() << "\n";
        return false;
    }
}

bool ProjectManager::CreateProjectStructure(const std::string& projectPath)
{
    try {
        // Create main project directory
        std::filesystem::create_directories(projectPath);
        
        // Create standard subdirectories
        std::filesystem::create_directories(projectPath + "/Assets");
        std::filesystem::create_directories(projectPath + "/Assets/Textures");
        std::filesystem::create_directories(projectPath + "/Assets/Models");
        std::filesystem::create_directories(projectPath + "/Assets/Materials");
        std::filesystem::create_directories(projectPath + "/Assets/Shaders");
        std::filesystem::create_directories(projectPath + "/Assets/Audio");
        std::filesystem::create_directories(projectPath + "/Scenes");
        std::filesystem::create_directories(projectPath + "/Scripts");
        std::filesystem::create_directories(projectPath + "/Temp");
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating project structure: " << e.what() << "\n";
        return false;
    }
}

} // namespace SparkEditor