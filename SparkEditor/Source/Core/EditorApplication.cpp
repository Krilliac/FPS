/**
 * @file EditorApplication.cpp
 * @brief Implementation of the main SparkEditor application
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorApplication.h"
#include <iostream>
#include <filesystem>

namespace SparkEditor {

EditorApplication::EditorApplication() {
    std::cout << "SparkEditor starting..." << std::endl;
}

EditorApplication::~EditorApplication() {
    std::cout << "SparkEditor shutting down..." << std::endl;
}

bool EditorApplication::Initialize(const EditorConfig& config) {
    std::cout << "Initializing SparkEditor..." << std::endl;
    
    m_config = config;
    
    // Create necessary directories
    try {
        std::filesystem::create_directories(config.layoutDirectory);
        std::filesystem::create_directories(config.logDirectory);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create directories: " << e.what() << std::endl;
        return false;
    }
    
    m_isInitialized = true;
    m_isRunning = true;
    
    std::cout << "SparkEditor initialized successfully" << std::endl;
    return true;
}

int EditorApplication::Run() {
    if (!m_isInitialized) {
        std::cerr << "EditorApplication not initialized!" << std::endl;
        return -1;
    }
    
    std::cout << "SparkEditor is running..." << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    
    // Simple run loop - replace with actual editor loop
    std::cin.get();
    
    return 0;
}

void EditorApplication::Shutdown() {
    m_isRunning = false;
    m_isInitialized = false;
    std::cout << "SparkEditor shutdown complete" << std::endl;
}

} // namespace SparkEditor
