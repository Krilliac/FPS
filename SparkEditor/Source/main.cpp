/**
 * @file main.cpp
 * @brief SparkEditor entry point
 * @author Spark Engine Team
 * @date 2025
 */

#include "Core/EditorApplication.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    try {
        // Create editor application
        auto app = std::make_unique<SparkEditor::EditorApplication>();
        
        // Initialize with basic configuration
        SparkEditor::EditorConfig config;
        config.projectPath = ".";
        config.enableLogging = true;
        config.startMaximized = true;
        
        if (!app->Initialize(config)) {
            std::cerr << "Failed to initialize SparkEditor" << std::endl;
            return -1;
        }
        
        // Run the editor
        int result = app->Run();
        
        // Cleanup
        app->Shutdown();
        
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "SparkEditor error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "SparkEditor: Unknown error occurred" << std::endl;
        return -1;
    }
}
